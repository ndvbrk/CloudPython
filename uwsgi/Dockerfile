# A dockerfile must always start by importing the base image.
# We use the keyword 'FROM' to do that.
# In our example, we want import the python image.
# So we write 'python' for the image name and 'latest' for the version.
FROM python:latest

RUN mkdir /Server
WORKDIR /Server

COPY src src
WORKDIR /Server/src
RUN python -m venv ./venv
COPY requirements.txt requirements.txt
RUN /bin/bash -c "source venv/bin/activate && pip install -r requirements.txt"

CMD [ "venv/bin/uwsgi", "uwsgi.ini"]


