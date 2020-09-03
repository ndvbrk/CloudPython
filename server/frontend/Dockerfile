# A dockerfile must always start by importing the base image.
# We use the keyword 'FROM' to do that.
# In our example, we want import the python image.
# So we write 'python' for the image name and 'latest' for the version.
FROM python:latest

RUN mkdir /home/frontend
RUN groupadd -r frontend && useradd --no-log-init -r -g frontend frontend
RUN chown frontend:frontend  /home/frontend
USER frontend
RUN mkdir /home/frontend/Server
WORKDIR /home/frontend/Server

RUN mkdir src
WORKDIR src
COPY requirements.txt requirements.txt
RUN python -m venv ./venv
RUN /bin/bash -c "source venv/bin/activate && pip install -r requirements.txt"
WORKDIR /home/frontend/Server
COPY src src
WORKDIR /home/frontend/Server/src
CMD [ "venv/bin/uwsgi", "uwsgi.ini"]


