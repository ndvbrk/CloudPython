include MakefileConfig.mk

YELLOW   = \033[0;33m 
GREEN    = \033[0;32m
RED = \033[0;31m
NO_COLOR    = \033[m
BOLD_TEXT := $(shell tput bold)
NORM_TEXT := $(shell tput sgr0)

CCX=g++
# Optimisation level
CCXFLAGS=-O2
# Avoid temporary files, speeding up builds
CCXFLAGS+=-pipe
# Run-time buffer overflow detection
CCXFLAGS+=-D_FORTIFY_SOURCE=2
# Run-time bounds checking for C++ strings and containers
CCXFLAGS+=-D_GLIBCXX_ASSERTIONS
# Full ASLR for executables
CCXFLAGS+=-fpie -Wl,-pie
# Stack smashing protector
CCXFLAGS+=-fstack-protector-all
# tells the compiler to use a canary to protect any function which
# declares a character array of 4 or more bytes on its stack, rather
# than the default of 8 or more bytes. 
CCXFLAGS+=--param ssp-buffer-size=4
# warnings for any functions that aren't going to get protected.
CCXFLAGS+=-Wstack-protector
# Turn on all warnings to help ensure the underlying code is secure
# and treat those warnings as errors
CCXFLAGS+=-Wall -Wextra -Werror
# Read-only segments after relocation
# https://www.redhat.com/en/blog/hardening-elf-binaries-using-relocation-read-only-relro
CCXFLAGS+=-Wl,-z,relro,-z,now
# Non-executable stack
CCXFLAGS+=-Wl,-z,noexecstack

LIBS=-L/usr/lib -L/usr/lib/x86_64-linux-gnu -lssl -lcrypto -lboost_system -pthread
BOOST_DIR=./boost_1_72_0
BOOST=$(BOOST_DIR)/README.md

BIN=./bin
RELEASE_DIR=$(BIN)/release
DEBUG_DIR=$(BIN)/debug
EXECUTABLE=$(RELEASE_DIR)/$(CLIENT_NAME)
DEBUG_EXECUTABLE=$(DEBUG_DIR)/$(CLIENT_NAME)
OBJ_DIR=$(RELEASE_DIR)/obj
OBJ_DIR_DEBUG=$(DEBUG_DIR)/obj

SRC_DIR=./src
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
HEADERS=$(wildcard $(SRC_DIR)/include/*.h)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
OBJ_DEBUG := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR_DEBUG)/%.o, $(SOURCES))

all: $(EXECUTABLE)
debug: $(DEBUG_EXECUTABLE)

clean:
	@rm -rf $(BIN)

run: $(EXECUTABLE) | $(TRUSTED_CERTIFICATE)
	@echo "$(RED)$(BOLD_TEXT)MAKE SURE YOU TRUST $(TRUSTED_CERTIFICATE) $(NORM_TEXT)$(NO_COLOR)"
	@$(EXECUTABLE) $(SERVER_HOSTNAME) $(TRUSTED_CERTIFICATE)|| true

run_debug: $(DEBUG_EXECUTABLE) | $(TRUSTED_CERTIFICATE)
	@echo "$(RED)$(BOLD_TEXT)MAKE SURE YOU TRUST $(TRUSTED_CERTIFICATE) $(NORM_TEXT)$(NO_COLOR)"
	@$(DEBUG_EXECUTABLE) $(SERVER_HOSTNAME) $(TRUSTED_CERTIFICATE)|| true

$(TRUSTED_CERTIFICATE): MakefileConfig.mk
	mkdir -p $(dir $(TRUSTED_CERTIFICATE))
	openssl s_client -connect $(SERVER_HOSTNAME):443 -showcerts < /dev/null 2>/dev/null | openssl x509 -outform PEM > $(TRUSTED_CERTIFICATE)

$(BOOST): 
	@wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz
	@tar -xzf boost_1_72_0.tar.gz
	@rm boost_1_72_0.tar.gz
	@sudo apt -qqq install libboost-system-dev --yes

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) $(BOOST)
	@mkdir -p $(OBJ_DIR)
	@echo "$(GREEN)       CC$(NO_COLOR) $(YELLOW)$@$(NO_COLOR)"
	@$(CCX) $(CCXFLAGS) -I$(BOOST_DIR)  -c $< -o $@

$(OBJ_DIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) $(BOOST)
	@mkdir -p $(OBJ_DIR_DEBUG)
	@echo "$(GREEN)       CC$(NO_COLOR) $(YELLOW)$@$(NO_COLOR)"
	@$(CCX) $(CCXFLAGS) -g -I$(BOOST_DIR) -c $< -o $@

$(EXECUTABLE): $(OBJS)
	@mkdir -p $(RELEASE_DIR)
	@echo "$(GREEN)       LD$(NO_COLOR) $(YELLOW)$@$(NO_COLOR)"
	@$(CCX) $(OBJS) -I$(BOOST_DIR) $(LIBS) -o $@ 
	@strip $(EXECUTABLE)

$(DEBUG_EXECUTABLE): $(OBJ_DEBUG)
	@mkdir -p $(DEBUG_DIR)
	@echo "$(GREEN)       LD$(NO_COLOR) $(YELLOW)$@$(NO_COLOR)"
	@$(CCX) $(OBJ_DEBUG) -I$(BOOST_DIR) $(LIBS) -o $@ 

format:
	@clang-format -i $(SOURCES) $(HEADERS)
