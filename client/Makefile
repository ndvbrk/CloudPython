CCX=g++

LIBS=-L/usr/lib -L/usr/lib/x86_64-linux-gnu -lssl -lcrypto -lboost_system -pthread
BOOST_DIR=./boost_1_72_0
BOOST_MARKER_FILE=$(BOOST_DIR)/README.md

CLIENT_NAME=client

EXECUTABLE=./bin/release/$(CLIENT_NAME)
DEBUG_EXECUTABLE=./bin/debug/$(CLIENT_NAME)
SRC_DIR=./src
SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)
INCLUDE_FILES=$(wildcard $(SRC_DIR)/include/*.h)


all: $(EXECUTABLE)
debug: $(DEBUG_EXECUTABLE)

run: $(EXECUTABLE)
	@$(EXECUTABLE)

run_debug: $(DEBUG_EXECUTABLE)
	@$(DEBUG_EXECUTABLE)

$(BOOST_MARKER_FILE): 
	@wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz
	@tar -xzf boost_1_72_0.tar.gz
	@rm boost_1_72_0.tar.gz

clean:
	@rm -rf ./bin

$(EXECUTABLE): $(BOOST_MARKER_FILE) $(SRC_FILES) $(INCLUDE_FILES)
	@sudo apt -qqq install libboost-system-dev --yes
	@mkdir -p ./bin/release
	$(CCX) $(SRC_FILES) -I$(BOOST_DIR) $(LIBS) -o $@ 

$(DEBUG_EXECUTABLE): $(BOOST_MARKER_FILE) $(SRC_FILES)  $(INCLUDE_FILES)
	@mkdir -p ./bin/debug
	$(CCX) -g $(SRC_FILES) -I$(BOOST_DIR) $(LIBS) -o $@ 


format:
	clang-format -i $(SRC_FILES) $(INCLUDE_FILES)
