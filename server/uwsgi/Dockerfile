# A dockerfile must always start by importing the base image.
# We use the keyword 'FROM' to do that.
# In our example, we want import the python image.
# So we write 'python' for the image name and 'latest' for the version.
FROM python:latest

RUN mkdir /Server
WORKDIR /Server

RUN mkdir src
WORKDIR src
COPY requirements.txt requirements.txt
RUN python -m venv ./venv
RUN /bin/bash -c "source venv/bin/activate && pip install -r requirements.txt"
WORKDIR /Server
COPY src src
WORKDIR /Server/src
CMD [ "venv/bin/uwsgi", "uwsgi.ini"]


