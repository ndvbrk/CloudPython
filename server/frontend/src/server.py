from os import urandom 
from flask import Flask, jsonify, request, url_for
from flask import Flask, url_for, render_template, redirect
from forms import *


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

@app.route('/signup', methods=('GET', 'POST'))
def signup():
    form = SignupForm()
    if form.validate_on_submit():
        email = form.email.data
        password = form.password.data
        return redirect(url_for('success'))
    return render_template('signup.jinja2',
                           form=form,
                           template='form-template')