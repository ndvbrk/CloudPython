
DOCKER_IMAGE_TAG=flask-uwsgi

.PHONY: all image run stop

all: image
	
run:
	sudo docker network ls|grep nginx2uwsgi >/dev/null ||  sudo docker network create nginx2uwsgi

	sudo docker run --rm -t -d								\
		--security-opt=no-new-privileges					\
		--cap-drop=ALL										\
		--network nginx2uwsgi 								\
		--name $(DOCKER_IMAGE_TAG)							\
		-v /var/run/docker.sock:/var/run/docker.sock 		\
		$(DOCKER_IMAGE_TAG)

	# -m 50M --memory-reservation 25M

stop:
	sudo docker kill $(DOCKER_IMAGE_TAG) > /dev/null || true

credentials.json:
	@echo "credentials.json file must be downloaded:"
	@echo "https://developers.google.com/gmail/api/quickstart/go?authuser=2"
	exit 1

token: credentials.json
	sudo docker run -v $(PWD):$(PWD) -it python:latest /bin/bash -c "cd $(PWD) && pip install --upgrade google-api-python-client google-auth-httplib2 google-auth-oauthlib && python $(PWD)/make_token.py"
	cp token.pickle ./src/gmail_token.pickle

image:
	sudo docker build -t flask-uwsgi .