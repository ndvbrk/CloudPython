COUNTRY=IL
CITY=Tel Aviv
ORGANISATION=CloudPython
UNIT=IT Department

keys: privatekey.key certificate.crt

privatekey.key certificate.crt: Makefile
	openssl req -x509 \
		-newkey rsa:2048 \
		-keyout privatekey.key \
		-out certificate.crt \
		-days 365 \
		-nodes \
		-subj "/C=$(COUNTRY)/L=$(CITY)/O=$(ORGANISATION)/OU=$(UNIT)"