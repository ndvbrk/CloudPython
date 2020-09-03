
all:
	make images -j2

nginx_image:
	+make -C nginx/
backend_image:
	+make -C backend/
frontend_image:
	+make -C frontend/

images: nginx_image backend_image

run: images
	make run -C frontend/
	make run -C backend/
	make run -C nginx/
	sudo docker ps

stop:
	make stop -C frontend/
	make stop -C backend/
	make stop -C nginx/

restart:
	make stop
	make run