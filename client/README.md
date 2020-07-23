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