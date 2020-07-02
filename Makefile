
all:
	make parallel -j2

nginx_image:
	+make -C nginx/
uwsgi_image:
	+make -C uwsgi/

parallel: nginx_image uwsgi_image


run:
	make run -C uwsgi/
	make run -C nginx/
	sudo docker ps

stop:
	make stop -C uwsgi/
	make stop -C nginx/
