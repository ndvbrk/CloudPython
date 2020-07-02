
DOCKER_IMAGE_TAG=flask-uwsgi
all:
	sudo docker build -t flask-uwsgi .

run:
	sudo docker network ls|grep nginx2uwsgi >/dev/null ||  sudo docker network create nginx2uwsgi

	sudo docker run --rm -t	-d								\
		--security-opt=no-new-privileges					\
		--cap-drop=ALL										\
		--network nginx2uwsgi 								\
		--name $(DOCKER_IMAGE_TAG)							\
		-v /var/run/docker.sock:/var/run/docker.sock 		\
		$(DOCKER_IMAGE_TAG)

	# -m 50M --memory-reservation 25M

stop:
	sudo docker kill $(DOCKER_IMAGE_TAG) > /dev/null || true