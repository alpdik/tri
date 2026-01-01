CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic

# Headers live under `include/`, including `include/tests/...`
INC      := -Iinclude -Iinclude/tests

EXE := tri
BUILD_DIR := build

SRC_CPP   := $(wildcard src/*.cpp)
DEMO_CPP  := $(wildcard include/tests/*.cpp)
SRCS      := $(SRC_CPP) $(DEMO_CPP)

OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all run clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

run: all
	./$(EXE)

clean:
	rm -rf $(BUILD_DIR) $(EXE)
