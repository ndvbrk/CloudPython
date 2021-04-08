from flask_wtf import FlaskForm
from wtforms import StringField, TextField, SubmitField, PasswordField, HiddenField, IntegerField, TextAreaField
from wtforms.validators import DataRequired, Length, Email, EqualTo


class SignupForm(FlaskForm):
    """Sign up for a user account."""

    email = StringField('Email', [
        Email(message='Not a valid email address.'),
        DataRequired()])
    password = PasswordField('Password', [
        DataRequired(message="Please enter a password."),
        Length(min=4, message=('Password is too short')),
    ])
    confirmPassword = PasswordField('Repeat Password', [
        EqualTo('password', message='Passwords must match.')
    ])
    # recaptcha = RecaptchaField()
    submit = SubmitField('Submit')


class ConfirmEmail(FlaskForm):
    """Email Confirmation form."""
    token = HiddenField('token')
    submit = SubmitField('Submit')


class LoginForm(FlaskForm):
    """Execute code please."""
    email = StringField('Email', [
        Email(message='Not a valid email address.'),
        DataRequired()])
    password = PasswordField('Password', [
        DataRequired(message="Please enter a password."),
        Length(min=4, message=('Password is too short')),
    ])
    totp = IntegerField('totp', [DataRequired()])
    # recaptcha = RecaptchaField()
    submit = SubmitField('Submit')


class ExecuteForm(FlaskForm):
    """Execute code please."""
    code = TextAreaField('Code', [DataRequired()])
    # recaptcha = RecaptchaField()
    submit = SubmitField('Submit')
