THIS_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))

credentials.json:
	@echo "credentials.json file must be downloaded:"
	@echo "https://developers.google.com/gmail/api/quickstart/python"
	exit 1

./src/gmail_token.pickle: credentials.json
	sudo docker run -v $(THIS_DIR):$(THIS_DIR) -it python:latest /bin/bash -c "cd $(THIS_DIR) && pip install --upgrade google-api-python-client google-auth-httplib2 google-auth-oauthlib && python ./make_token.py"
	mv token.pickle ./src/gmail_token.pickle