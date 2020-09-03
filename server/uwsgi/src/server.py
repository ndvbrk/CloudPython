import os
import functools
import secrets
from flask import Flask, jsonify, request, url_for, render_template, redirect
from itsdangerous import URLSafeTimedSerializer, SignatureExpired, BadSignature
from passlib.hash import pbkdf2_sha256
from gmail import Gmail
from docker_wrap import run_code
from errors import *
import pyotp
import qrcode
import base64
import io


SECRET_KEY = secrets.token_hex(64)
tokenizer = URLSafeTimedSerializer(SECRET_KEY)
EMAIL_CONFIRMATION_SALT = 'email-confirmation-salt'
ADMIN_APPROVAL_SALT = 'admin-user-approval'
ADMINS_EMAIL = 'ndvbrk@gmail.com'
EXECUTION_TIMEOUT_SECONDS = 10
USER_OUT_MAX_SIZE = 4096 # Arbitrary limit
email_service = Gmail()

app = Flask(__name__)
app.config['SECRET_KEY'] = os.urandom(32)


def create_button(link, text):
    html = """<table cellspacing="0" cellpadding="0">"""
    html += """<tr>"""
    html += """<td align="center" width="300" height="40" bgcolor="#000091" style="-webkit-border-radius: 5px; -moz-border-radius: 5px; border-radius: 5px; color: #ffffff; display: block;">"""
    html += f"""<a href="{link}" style="font-size:16px; font-weight: bold; font-family: Helvetica, Arial, sans-serif; text-decoration: none; line-height:40px; width:100%; display:inline-block"><span style="color: #FFFFFF">{text}</span></a>"""
    html += """</td>"""
    html += """</tr>"""
    html += """</table>"""
    return html


def rest_api(f):
    @functools.wraps(f)
    def inner(*args, **kwargs):
        result, httpcode = f(*args, **kwargs)
        if type(result) == str:
            return {'error': result}, httpcode
        elif type(result) == dict:
            # Combine result dict with this dict
            # Users may override the 'ok' value by
            # including 'error' in their results
            d = {'error': 'ok'}
            d.update(result)
            return d, httpcode
        else:
            return '', httpcode

    return inner


class User:

    def __init__(self, email, password):
        self.email = email
        self.password_hash = pbkdf2_sha256.hash(password)
        self.email_verified = False
        self.await_approval = False
        self.approved = False
        self.totp = self.make_totp()

    @staticmethod
    def make_totp():
        secret = pyotp.random_base32()
        totp = pyotp.TOTP(secret)
        return totp

    def verify_totp(self, number):
        return self.totp.verify(number)

    def send_verification_email(self):
        token = tokenizer.dumps(self.email, salt=EMAIL_CONFIRMATION_SALT)
        confirm_url = url_for('confirm_email', token=token, _external=True)

        html_content = create_button(
            confirm_url, 'Click Here to activate your account')
        email_service.send(
            self.email, 'Activate your account with Cloud Python', html_content)

    def request_approval_from_admin(self):
        if self.await_approval:
            return
        token = tokenizer.dumps(self.email, salt=ADMIN_APPROVAL_SALT)
        approve_url = url_for('approve_user', token=token, _external=True)

        html_content = f'The user {self.email} has requested to use your platform.'
        html_content += '<br>'
        html_content += create_button(
            approve_url, 'Clicking here will activate his account')

        email_service.send(ADMINS_EMAIL, f'Cloud Python service access request: {self.email}', html_content)

        self.await_approval = True

    def send_welcome_email(self):
        email_service.send(
            self.email, 'Cloud Python: Account activated by administrator', '')

    def assert_execute(self, password, totp):
        if pbkdf2_sha256.verify(password, self.password_hash) is not True:
            raise Unauthorised()

        if not self.email_verified:
            raise Unauthorised()

        if not self.verify_totp(totp):
            raise Unauthorised()

        if not self.approved:
            # Consider FORBIDDEN instead?
            raise Unauthorised()


class UserDB:

    def __init__(self):
        self.all_users = dict()

    def get_user(self, email):
        if email not in self.all_users:
            raise BadRequest()

        return self.all_users[email]

    def register(self, email, password):
        """
        Runs in response to a user registering for the first time
        """
        if email in self.all_users:
            # https://stackoverflow.com/questions/9269040
            raise Created()

        user = User(email, password)
        self.all_users[email] = user

        user.send_verification_email()

        raise Created()

    def confirm_email(self, token):
        """
        Runs in response to a user registering for the first time
        """
        ten_minutes = 60 * 10
        try:
            email = tokenizer.loads(
                token, max_age=ten_minutes, salt=EMAIL_CONFIRMATION_SALT)
        except (SignatureExpired, BadSignature) as e:
            raise BadRequest()

        user = self.get_user(email)
        user.request_approval_from_admin()

        user.email_verified = True
        if not user.approved:
            response = ['Account is awaiting approval by site administator']
        else:
            # Happens if the user clicks the same link
            # again, after already being approved by admin
            response = ['Account is ready to use service']

        totp_url = user.totp.provisioning_uri('Cloud Python')
        qrcode_image = qrcode.make(totp_url)
        file_like_object = io.BytesIO()
        qrcode_image.save(file_like_object, format="png")
        image_bytes = file_like_object.getvalue()
        encoded = base64.b64encode(image_bytes).decode('ascii')
        image_html = f'<img src="data:image/png;base64,{encoded}"/>'

        response += [
            'Here is a QR code you should load into a 2FA authentication App']
        response += [image_html]
        response += [f'<a href="{totp_url}">Or you may just click this provisioning URI</a>']
        return '<br>'.join(response)

    def process_admin_approval(self, token):
        ten_minutes = 60 * 10
        try:
            email = tokenizer.loads(
                token, max_age=ten_minutes, salt=ADMIN_APPROVAL_SALT)
        except (SignatureExpired, BadSignature) as e:
            raise BadRequest()

        if email not in self.all_users:
            # Dead code, in theory.
            # If he didnt register, how come it is approved?
            # The server generates this token only after email confirmation
            # Code is being extra defensive here.
            return f'Error: {email} did not register'

        user = self.all_users[email]

        if user.email_verified is not True:
            # Dead code, in theory.
            # If he didnt register, how come it is approved?
            # The server generates this token only after email confirmation
            # Code is being extra defensive here.
            return f'Error: {email} is not yet verified'

        user.approved = True

        user.send_welcome_email()

        return f'{email}  has been approved to use our services'

    def assert_execute(self, email, password, totp):
        user = self.get_user(email)

        return user.assert_execute(password, totp)


user_database = UserDB()


@app.route('/api/execute', methods=['POST'])
@rest_api
@catch_errors
def api_execute():
    json_data = DictWrapper(request.get_json(force=False))
    email = json_data['email']
    password = json_data['password']
    totp = json_data['totp']
    try:
        code = bytes.fromhex(json_data['code'])
    except ValueError:
        raise BadRequest()
    
    user_database.assert_execute(email, password, totp)

    try:
        result = run_code(code, EXECUTION_TIMEOUT_SECONDS, USER_OUT_MAX_SIZE)
        return result
    except TimeoutError:
        return 'request timed out'


@app.route('/api/register', methods=['POST'])
@rest_api
@catch_errors
def api_register():
    json_data = DictWrapper(request.get_json(force=False))
    email = json_data['email']
    password = json_data['password']
    return user_database.register(email, password)


@app.route('/api/confirm_email/<token>', methods=['GET'])
@catch_errors
def confirm_email(token):
    return user_database.confirm_email(token)


@app.route('/api/admin/approve_user/<token>', methods=['GET'])
@catch_errors
def approve_user(token):
    return user_database.process_admin_approval(token)

