# Compiler
CXX = g++
CC = gcc  # Add this line for C files like glad.c

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -w -Wextra -Iinclude -I/usr/include/freetype2
CFLAGS = -Iinclude -I/usr/include/freetype2  # Add flags for C files
DEBUG ?= 0

# Linker flags
LDFLAGS = -lGL -lglfw -lfreetype

# Directories
SRC_DIR = src
OBJ_DIR = build
INC_DIR = include
SHADER_DIR = shaders

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp) $(SRC_DIR)/glad.c main.cpp
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(OBJS))  # Add rule for .c to .o conversion

# Executable
TARGET = $(OBJ_DIR)/app

# Shader files
SHADERS = $(wildcard $(SHADER_DIR)/*.glsl)

# Default target
all: $(TARGET) shaders

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

# Compile C++ source files into object files
$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# Compile C source files into object files
$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# Copy shaders to build directory
shaders:
	mkdir -p $(OBJ_DIR)
	cp $(SHADERS) $(OBJ_DIR)/

# Clean up build files
clean:
	rm -rf $(OBJ_DIR)

# Debug option
debug: DEBUG = 1
debug: CXXFLAGS += -g
debug: all
