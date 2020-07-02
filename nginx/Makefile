DOCKER_IMAGE_TAG=mynginx

COUNTRY=IL
CITY=Tel Aviv
ORGANISATION=CloudPython
UNIT=IT Department

.PHONY: all image run stop

all: image

image: keys
	sudo docker build -t $(DOCKER_IMAGE_TAG) .

keys: privatekey.key certificate.crt

privatekey.key certificate.crt: Makefile
	openssl req -x509 \
		-newkey rsa:2048 \
		-keyout privatekey.key \
		-out certificate.crt \
		-days 365 \
		-nodes \
		-subj "/C=$(COUNTRY)/L=$(CITY)/O=$(ORGANISATION)/OU=$(UNIT)"

run: all
	sudo docker network ls|grep nginx2uwsgi >/dev/null ||  sudo docker network create nginx2uwsgi
	sudo docker run --rm -t	-d		\
			-m 50M --memory-reservation 25M	\
			--network nginx2uwsgi			\
			--name $(DOCKER_IMAGE_TAG)		\
			--cap-drop=ALL					\
			--security-opt=no-new-privileges\
			-p 443:5000						\
			$(DOCKER_IMAGE_TAG)

stop:
	sudo docker kill $(DOCKER_IMAGE_TAG) > /dev/null || true