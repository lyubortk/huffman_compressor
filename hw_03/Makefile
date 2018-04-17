CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -pedantic -Werror -std=c++11 -I$(INCLDIR)

HUF_INCL_DIR = src
TEST_INCL_DIR = test
HUF_EXE = huffman
TEST_EXE = test
HUF_DIR = src
TEST_DIR = test
BIN_DIR = bin

HUF_OBJECTS = $(patsubst $(HUF_DIR)/%.cpp,$(BIN_DIR)/%.o,$(wildcard $(HUF_DIR)/*.cpp))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%.o,$(wildcard $(TEST_DIR)/*.cpp))
HUF_INCLUDES = $(wildcard $(HUF_INCL_DIR)/*.h)
TEST_INCLUDES = $(wildcard $(TEST_INCL_DIR)/*.h)

all: $(EXE)

$(HUF_EXE): $(BIN_DIR) $(HUF_OBJECTS)
	$(CXX) $(HUF_OBJECTS) -o $(HUF_EXE)

$(TEST_EXE): $(BIN_DIR) $(TEST_OBJECTS)
	$(CXX) $(TEST_OBJECTS) -o $(TEST_EXE)

$(HUF_OBJECTS): $(patsubst $(BIN_DIR)/%.o,$(HUF_DIR)/%.cpp,$@) $(HUF_INCLUDES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TEST_OBJECTS): $(patsubst $(BIN_DIR)/%.o,$(TES_DIR)/%.cpp,$@) $(TEST_INCLUDES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR)

.PHONY: clean all
