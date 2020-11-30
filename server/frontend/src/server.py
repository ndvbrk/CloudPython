from os import urandom 
from flask import Flask, jsonify, request, url_for
from flask import Flask, url_for, render_template, redirect
from forms import *
import requests
import json
from http import HTTPStatus

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
    return render_template('show_text.jinja2', 
                title=status.name,
                page_title=status.name.replace('_', ' '),
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
        json ={'email': email, 'password': password})
        return render_from_backend(response)            
    return render_template('signup.jinja2',
                           form=form,
                           template='form-template')