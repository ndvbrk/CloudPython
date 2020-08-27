include MakefileConfig.mk

ifneq "$(filter %debug,$(MAKECMDGOALS))" ""
BUILD=debug
CXXFLAGS+=-g
else
BUILD=release
endif

YELLOW   = \033[0;33m 
GREEN    = \033[0;32m
RED = \033[0;31m
NO_COLOR    = \033[m
BOLD_TEXT := $(shell tput bold)
NORM_TEXT := $(shell tput sgr0)

# Used https://github.com/slimm609/checksec.sh to verify end product

CCX=g++
# Optimisation level
CCXFLAGS+=-O2
# Avoid temporary files, speeding up builds
CCXFLAGS+=-pipe
# Run-time buffer overflow detection
CCXFLAGS+=-D_FORTIFY_SOURCE=2
# Run-time bounds checking for C++ strings and containers
CCXFLAGS+=-D_GLIBCXX_ASSERTIONS
# Full ASLR for executables
#CCXFLAGS+=-fPIC -Wl,-pie
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


LIBS=-L/usr/lib -L/usr/lib/x86_64-linux-gnu -l:libssl.a -l:libcrypto.a -l:libboost_system.a -pthread -ldl
BOOST_DIR=./boost_1_72_0
BOOST=$(BOOST_DIR)/README.md

BIN=./bin
EXECUTABLE=$(BIN)/$(BUILD)/$(CLIENT_NAME)
OBJ_DIR=$(BIN)/$(BUILD)/obj

SRC_DIR=./src
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
HEADERS=$(wildcard $(SRC_DIR)/include/*.h)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
DEPS:= $(OBJS:.o=.d)
ARGUMENTS := $(SERVER_HOSTNAME) $(TRUSTED_CERTIFICATE) $(SCRIPT_TO_RUN)

all: $(EXECUTABLE)
debug: $(EXECUTABLE)

clean:
	@rm -rf $(BIN)

%.d: 
	@mkdir -p $(dir $@)
	@$(CCX) -MM -MT '$(@:.d=.o)' $(SRC_DIR)/$(basename $(notdir $@)).cpp > $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

run_debug: run

run: $(EXECUTABLE) | $(TRUSTED_CERTIFICATE)
	@echo "$(RED)$(BOLD_TEXT)MAKE SURE YOU TRUST $(TRUSTED_CERTIFICATE) $(NORM_TEXT)$(NO_COLOR)"
	@echo "Executing: $(RED)$(EXECUTABLE) $(ARGUMENTS)$(NO_COLOR)"
	@$(EXECUTABLE) $(ARGUMENTS) || true


$(TRUSTED_CERTIFICATE): MakefileConfig.mk
	mkdir -p $(dir $(TRUSTED_CERTIFICATE))
	openssl s_client -connect $(SERVER_HOSTNAME):443 -showcerts < /dev/null 2>/dev/null | openssl x509 -outform PEM > $(TRUSTED_CERTIFICATE)

$(BOOST): 
	@wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz
	@tar -xzf boost_1_72_0.tar.gz
	@rm boost_1_72_0.tar.gz
	@sudo apt -qqq install libboost-system-dev --yes

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(BOOST)
	@mkdir -p $(OBJ_DIR)
	@echo "$(GREEN)       CC$(NO_COLOR) $(YELLOW)$@$(NO_COLOR)"
	@$(CCX) $(CCXFLAGS) -I$(BOOST_DIR)  -c $< -o $@

$(EXECUTABLE): $(OBJS)
	@echo "$(GREEN)       LD$(NO_COLOR) $(YELLOW)$@$(NO_COLOR)"
	@$(CCX) $(CCXFLAGS) $(OBJS) -I$(BOOST_DIR) $(LIBS) -o $@ 
	@strip $(EXECUTABLE)

format:
	@clang-format -i $(SOURCES) $(HEADERS)
