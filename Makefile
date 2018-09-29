CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -Wshadow -pedantic -Werror -std=c++17 -I$(HUF_INCL_DIR) -I$(TEST_INCL_DIR)

HUF_INCL_DIR = includes
TEST_INCL_DIR = test_includes
HUF_EXE = huffman
TEST_EXE = test
HUF_DIR = src
TEST_DIR = test_src
BIN_DIR = bin

HUF_OBJECTS = $(patsubst $(HUF_DIR)/%.cpp,$(BIN_DIR)/%.o,$(wildcard $(HUF_DIR)/*.cpp))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%.o,$(wildcard $(TEST_DIR)/*.cpp))
HUF_INCLUDES = $(wildcard $(HUF_INCL_DIR)/*.h)
TEST_INCLUDES = $(wildcard $(TEST_INCL_DIR)/*.h)

all: $(HUF_EXE)

$(HUF_EXE): $(BIN_DIR) $(HUF_OBJECTS)
	$(CXX) $(HUF_OBJECTS) -o $(HUF_EXE)

$(TEST_EXE): $(BIN_DIR) $(TEST_OBJECTS) $(HUF_OBJECTS)
	$(CXX) $(TEST_OBJECTS) $(patsubst $(BIN_DIR)/main.o,,$(HUF_OBJECTS)) -o $(TEST_EXE)

.SECONDEXPANSION:
$(HUF_OBJECTS): $$(patsubst $(BIN_DIR)/%.o,$(HUF_DIR)/%.cpp,$$@) $(HUF_INCLUDES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TEST_OBJECTS): $$(patsubst $(BIN_DIR)/%.o,$(TEST_DIR)/%.cpp,$$@) $(TEST_INCLUDES) $(HUF_INCLUDES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

.PHONY: clean all
