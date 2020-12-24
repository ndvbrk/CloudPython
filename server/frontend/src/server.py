from os import urandom 
from flask import Flask, jsonify, request, url_for
from flask import Flask, url_for, render_template, redirect
from forms import *
import requests
import json
from http import HTTPStatus
import qrcode
import base64
import io

app = Flask(__name__)
app.config['SECRET_KEY'] = urandom(32)


@app.route('/contact', methods=('GET', 'POST'))
def contact():
    form = ContactForm()
    if form.validate_on_submit():
        email = form.email.data
        name = form.name.data
        body = form.body.data
        return redirect(url_for('success'))
    return render_template('contact.jinja2', form=form, template='form-template')

@app.route('/success', methods=('GET',))
def success():
    return render_template('show_text.jinja2', 
                           title='Success',
                           page_title='Success!',
                           page_text='You\'ve submitted a valid form!')

def render_from_backend(response):
    json_response = json.loads(response.text)
    status = HTTPStatus(response.status_code)
    title = status.name.replace('_', ' ')
    return render_template('show_text.jinja2', 
                title=title,
                page_title=title,
                page_text=json_response["error"]
    )

@app.route('/signup', methods=('GET', 'POST'))
def signup():
    form = SignupForm()
    if form.validate_on_submit():
        email = form.email.data
        password = form.password.data
        server ="http://mynginx:8000/api/register"
        response = requests.post(server, 
                                 json={'email': email, 'password': password},
                                 headers={'Host': request.host})
        return render_from_backend(response)            
    return render_template('signup.jinja2',
                           form=form,
                           template='form-template')

@app.route('/confirm_email/<token>', methods=('GET',))
def confirm_email(token):
    # TODO: Ask permission before sending request
    server = "http://mynginx:8000/api/confirm_email"
    backend_response = requests.post(
        server, json={'token': token}, headers={'Host': request.host})
    if backend_response.status_code != HTTPStatus.OK:
        return render_from_backend(backend_response)

    totp_url = json.loads(backend_response.text)["error"]

    qrcode_image = qrcode.make(totp_url)
    file_like_object = io.BytesIO()
    qrcode_image.save(file_like_object, format="png")
    image_bytes = file_like_object.getvalue()
    encoded = base64.b64encode(image_bytes).decode('ascii')
    image_html = f'<img src="data:image/png;base64,{encoded}"/>'

    page_body = ['<p> Here is a QR code you should load into a 2FA authentication App</p>']
    page_body += [image_html]
    page_body += [f'<a href="{totp_url}">Or you may just click this provisioning URI</a>']
    page_body_merged = '\n<br>\n'.join(page_body)

    title = 'Email Confirmed'
    return render_template('qr.jinja2', 
                title=title,
                page_title=title,
                page_body=page_body_merged
    )
