CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O3
BUILD_DIR := build
SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
EXECUTABLE := $(BUILD_DIR)/main

all: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

ifeq ($(OS),Windows_NT)
    CLEAN_COMMAND := del /Q /S
else
    CLEAN_COMMAND := rm -rf
endif

clean:
	$(CLEAN_COMMAND) build

.PHONY: all clean