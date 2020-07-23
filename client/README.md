# Cloud Python C++ Client

A C++ implementation of A CLI based API client for Cloud Python.

Based on Boost for Beast (http) + PropertyTree (JSON) and with openssl for TLS.

## Dependencies

* openssl

```sudo apt install openssl```


* boost library (1.72)

```sudo apt install libboost-system-dev```


# Usage

| Command | Result |
| ------- | ------ |
| `make`  | Build the client at `./bin/release/client` |
| `make clean`  | Delete everything under `./bin` |
| `make debug`  | Build the client at `./bin/debug/client` |
| `make run`  | Run the release client using parameters in MakefileConfig.mk. <br/>**Fetches the certificate with an unsafe download** |
| `make run_debug`  | Run the debug client using parameters in MakefileConfig.mk.<br/> **Fetches the certificate with an unsafe download** |
| `make format`  | Formats the source files with clang-format (**requires installation**) |


To really run securly:

1. `make`
2. Find a secure way to download the Server's certificate (This is a limitation of using self signed certificates)

3. Invoke:
   `./bin/release/client SERVER_HOSTNAME TRUSTED_CERTIFICATE_PATH PYTHON_SCRIPT_PATH`

    * Example: `./bin/release/client CloudPython.com /bin/selfsigned.crt ./script.py`