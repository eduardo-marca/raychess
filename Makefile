# Project name
PROJECT_NAME := RayChess

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -MMD -MP

# Raylib and system libraries
LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin

# Find all .cpp files recursively
SRC := $(shell find $(SRC_DIR) -type f -name "*.cpp")

# Generate matching .o paths in BUILD_DIR keeping subfolders
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

TARGET := $(BIN_DIR)/$(PROJECT_NAME)

all: $(TARGET)

# Link
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LIBS)

# Compile rule — auto-create subdirs inside build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they don't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the game after build
run: all
	./$(TARGET)

# Include dependency files
-include $(DEP)
