import base64
import pickle
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from googleapiclient.discovery import build

SCOPES = ['https://www.googleapis.com/auth/gmail.send']

class Gmail:
    def __init__(self):
        with open('data/gmail_token.pickle', 'rb') as token:
            creds = pickle.load(token)
    
        self.service = build('gmail', 'v1', credentials=creds)

    def send(self, to, msg_subject, message_body):
        message = MIMEMultipart('alternative')
        message['to'] = to
        message['subject'] = msg_subject

        html_part = MIMEText(message_body, 'html')
        message.attach(html_part)

        # Base 64 encode
        b64_bytes = base64.urlsafe_b64encode(message.as_bytes())
        b64_string = b64_bytes.decode()
        message_raw = {'raw': b64_string}
        
        message = (self.service.users().messages().send(userId='me', body=message_raw).execute())
        return message
