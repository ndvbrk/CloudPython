import base64
import io
import json
from http import HTTPStatus
from os import urandom

import qrcode
import requests
from flask import Flask, jsonify, redirect, render_template, request, url_for
from flask_bootstrap import Bootstrap

import forms

app = Flask(__name__)
app.config['SECRET_KEY'] = urandom(32)

bootstrap = Bootstrap(app)


@app.route('/')
def index():
    return render_template('index.html')


def render_from_backend(response):
    json_response = json.loads(response.text)
    status = HTTPStatus(response.status_code)
    title = status.name.replace('_', ' ')
    return render_template('show_text.jinja2',
                           title=title,
                           page_title=title,
                           page_body=f'<p>{json_response["error"]}</p>'
                           )


@app.route('/signup', methods=('GET', 'POST'))
def signup():
    form = forms.SignupForm()
    if form.validate_on_submit():
        email = form.email.data
        password = form.password.data
        server = "http://mynginx:8000/api/register"
        response = requests.post(server,
                                 json={'email': email, 'password': password},
                                 headers={'Host': request.host})
        return render_from_backend(response)
    return render_template('signup.jinja2',
                           form=form,
                           template='form-template')


def render_totp_url(totp_url):
    qrcode_image = qrcode.make(totp_url)
    file_like_object = io.BytesIO()
    qrcode_image.save(file_like_object, format="png")
    image_bytes = file_like_object.getvalue()
    encoded = base64.b64encode(image_bytes).decode('ascii')
    image_html = f'<img src="data:image/png;base64,{encoded}"/>'

    page_body = [
        '<p> Here is a QR code you should load into a 2FA authentication App</p>']
    page_body += [image_html]
    page_body += [f'<a href="{totp_url}">Or you may just click this provisioning URI</a>']
    page_body_merged = '\n<br>\n'.join(page_body)

    title = 'Email Confirmed'
    return render_template('show_text.jinja2',
                           title=title,
                           page_title=title,
                           page_body=page_body_merged
                           )


@app.route('/confirm_email', methods=('POST',), defaults={'token': None})
@app.route('/confirm_email/<token>', methods=('GET',))
def confirm_email_submit(token):
    form = forms.ConfirmEmail()
    if form.validate_on_submit():
        server = "http://mynginx:8000/api/confirm_email"
        token = form.token.data
        backend_response = requests.post(
            server, json={'token': token}, headers={'Host': request.host})
        if backend_response.status_code != HTTPStatus.OK:
            return render_from_backend(backend_response)

        totp_url = json.loads(backend_response.text)["error"]
        return render_totp_url(totp_url)
    else:
        if token is None:
            return render_template('show_text.jinja2',
                                   title='404',
                                   page_title='Page not found',
                                   page_body='Bye bye'
                                   )
        server = "http://mynginx:8000/api/before_confirm_email"
        backend_response = requests.post(
            server, json={'token': token}, headers={'Host': request.host})
        if backend_response.status_code != HTTPStatus.OK:
            return render_from_backend(backend_response)
        email = json.loads(backend_response.text)["error"]
        form.token.data = token
        return render_template('confirm_email.jinja2',
                               form=form,
                               email=email,
                               template='form-template')


@ app.route('/admin/approve_user/<token>', methods=('GET',))
@ app.route('/admin/approve_user', methods=('POST',), defaults={'token': None})
def approve_user_submit(token):
    form = forms.ConfirmEmail()
    if form.validate_on_submit():
        token = form.token.data
        server = "http://mynginx:8000/api/admin/approve_user"
        backend_response = requests.post(
            server, json={'token': token}, headers={'Host': request.host})
        return render_from_backend(backend_response)
    else:
        if token is None:
            return render_template('show_text.jinja2',
                                   title='404',
                                   page_title='Page not found',
                                   page_body='Bye bye'
                                   )
        server = "http://mynginx:8000/api/admin/before_approve_user"
        backend_response = requests.post(
            server, json={'token': token}, headers={'Host': request.host})
        if backend_response.status_code != HTTPStatus.OK:
            return render_from_backend(backend_response)
        email = json.loads(backend_response.text)["error"]
        form.token.data = token
        return render_template('approve_user.jinja2',
                               form=form,
                               email=email,
                               template='form-template')
