from flask_wtf import FlaskForm
from wtforms import StringField, TextField, SubmitField, PasswordField
from wtforms.validators import DataRequired, Length, Email, EqualTo

class ContactForm(FlaskForm):
    """Contact form."""
    name = StringField('Name', [
        DataRequired()])
    email = StringField('Email', [
        Email(message=('Not a valid email address.')),
        DataRequired()])
    body = TextField('Message', [
        DataRequired(),
        Length(min=4, message=('Your message is too short.'))])
    #recaptcha = RecaptchaField()
    submit = SubmitField('Submit')

class SignupForm(FlaskForm):
    """Sign up for a user account."""

    email = StringField('Email', [
        Email(message='Not a valid email address.'),
        DataRequired()])
    password = PasswordField('Password', [
        DataRequired(message="Please enter a password."),
    ])
    confirmPassword = PasswordField('Repeat Password', [
            EqualTo('password', message='Passwords must match.')
            ])
    # recaptcha = RecaptchaField()
    submit = SubmitField('Submit')