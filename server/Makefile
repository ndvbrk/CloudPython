
all:
	make images -j2

nginx_image:
	+make -C nginx/
uwsgi_image:
	+make -C uwsgi/

images: nginx_image uwsgi_image

run: images
	make run -C uwsgi/
	make run -C nginx/
	sudo docker ps

stop:
	make stop -C uwsgi/
	make stop -C nginx/

restart:
	make stop
	make run