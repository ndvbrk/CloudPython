nginx_requirements:
	+make -C nginx/ keys
backend_requirements:
	+make -C backend/ ./src/gmail_token.pickle

requirements:
	+make nginx_requirements -s
	+make backend_requirements -s

run: requirements
	docker-compose up -d --build

stop:
	docker-compose kill
	docker-compose down --volumes

restart:
	make stop
	make run

logs:
	docker-compose logs