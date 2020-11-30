
DOCKER_IMAGE_TAG=flask-frontend

.PHONY: all image run stop

all: image
	
run: image
	sudo docker network ls|grep nginx2uwsgi >/dev/null ||  sudo docker network create nginx2uwsgi

	sudo docker run -t -d 								\
		--name $(DOCKER_IMAGE_TAG)							\
		--network nginx2uwsgi 								\
		--cap-drop=ALL										\
		--security-opt=no-new-privileges					\
		-v /var/run/docker.sock:/var/run/docker.sock 		\
		$(DOCKER_IMAGE_TAG)

stop:
	sudo docker kill $(DOCKER_IMAGE_TAG) > /dev/null || true
	sudo docker rm $(DOCKER_IMAGE_TAG)  > /dev/null || true

image:
	sudo docker build -t $(DOCKER_IMAGE_TAG) .

logs:
	sudo docker logs -t $(DOCKER_IMAGE_TAG)