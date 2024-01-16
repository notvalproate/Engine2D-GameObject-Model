CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O3
BUILD_DIR := build
TARGET := main
SRCS := main.cpp Core.cpp Component.cpp
OBJS := $(addprefix $(BUILD_DIR)/,$(SRCS:.cpp=.o))

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: %.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

ifeq ($(OS),Windows_NT)
    CLEAN_COMMAND := del /Q /S
else
    CLEAN_COMMAND := rm -rf
endif

clean:
	$(CLEAN_COMMAND) build

.PHONY: all clean