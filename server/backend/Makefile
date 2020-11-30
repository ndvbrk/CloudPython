
DOCKER_IMAGE_TAG=flask-backend
THIS_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))

.PHONY: all image run stop

all: image
	
run: image
	sudo docker network ls|grep nginx2uwsgi >/dev/null ||  sudo docker network create nginx2uwsgi

	sudo docker run  -t -d								\
		--name $(DOCKER_IMAGE_TAG)							\
		--network nginx2uwsgi 								\
		--cap-drop=ALL										\
		--security-opt=no-new-privileges					\
		-v /var/run/docker.sock:/var/run/docker.sock 		\
		$(DOCKER_IMAGE_TAG)

stop:
	sudo docker kill $(DOCKER_IMAGE_TAG) > /dev/null || true
	sudo docker rm $(DOCKER_IMAGE_TAG)

credentials.json:
	@echo "credentials.json file must be downloaded:"
	@echo "https://developers.google.com/gmail/api/quickstart/go?authuser=2"
	exit 1

./src/gmail_token.pickle: credentials.json
	sudo docker run -v $(THIS_DIR):$(THIS_DIR) -it python:latest /bin/bash -c "cd $(THIS_DIR) && pip install --upgrade google-api-python-client google-auth-httplib2 google-auth-oauthlib && python ./make_token.py"
	mv token.pickle ./src/gmail_token.pickle

image: ./src/gmail_token.pickle
	sudo docker build -t $(DOCKER_IMAGE_TAG) .