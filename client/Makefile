YELLOW   = \033[0;33m 
GREEN    = \033[0;32m
RED = \033[0;31m
NO_COLOR    = \033[m

CCX=g++
LIBS=-L/usr/lib -L/usr/lib/x86_64-linux-gnu -lssl -lcrypto -lboost_system -pthread
BOOST_DIR=./boost_1_72_0
BOOST=$(BOOST_DIR)/README.md

CLIENT_NAME=client

EXECUTABLE=./bin/release/$(CLIENT_NAME)
DEBUG_EXECUTABLE=./bin/debug/$(CLIENT_NAME)
SRC_DIR=./src
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
HEADERS=$(wildcard $(SRC_DIR)/include/*.h)
OBJ_DIR=./bin/release/obj
OBJ_DIR_DEBUG=./bin/debug/obj
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
OBJECTS_DEBUG := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR_DEBUG)/%.o, $(SOURCES))

all: $(EXECUTABLE)
debug: $(DEBUG_EXECUTABLE)

run: $(EXECUTABLE)
	@$(EXECUTABLE) || true

run_debug: $(DEBUG_EXECUTABLE)
	@$(DEBUG_EXECUTABLE) || true

$(BOOST): 
	@wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz
	@tar -xzf boost_1_72_0.tar.gz
	@rm boost_1_72_0.tar.gz
	@sudo apt -qqq install libboost-system-dev --yes

clean:
	@rm -rf ./bin

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) $(BOOST)
	@mkdir -p $(OBJ_DIR)
	@echo "$(GREEN) CC$(NO_COLOR): $(YELLOW)$@$(NO_COLOR)"
	@$(CCX) -I$(BOOST_DIR)  -c $< -o $@

$(OBJ_DIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) $(BOOST)
	@mkdir -p $(OBJ_DIR_DEBUG)
	@echo "$(GREEN) CC$(NO_COLOR): $(YELLOW)$@$(NO_COLOR)"
	@$(CCX) -g -I$(BOOST_DIR) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p ./bin/release
	@echo "$(GREEN) LD$(NO_COLOR): $(YELLOW)$@$(NO_COLOR)"
	@$(CCX) $(OBJECTS) -I$(BOOST_DIR) $(LIBS) -o $@ 

$(DEBUG_EXECUTABLE): $(OBJECTS_DEBUG)
	@mkdir -p ./bin/debug
	@echo "$(GREEN) LD$(NO_COLOR): $(YELLOW)$@$(NO_COLOR)"
	@$(CCX) $(OBJECTS_DEBUG) -I$(BOOST_DIR) $(LIBS) -o $@ 

format:
	@clang-format -i $(SOURCES) $(HEADERS)
