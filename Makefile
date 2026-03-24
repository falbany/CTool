# ==========================================
# CTool & CBridge Makefile
# Target: Shared Library (DLL/SO)
# ==========================================

# Compiler settings
CC       := gcc
CXX      := g++

# -DCTOOL_DYNAMIC      : Deactivate the default static compilation mode of libctool.h
# -DBUILDING_CTOOL_DLL : Indicate that we want to export symbols (dllexport)
DLL_FLAGS := -DCTOOL_DYNAMIC -DBUILDING_CTOOL_DLL

CFLAGS   := -std=c99 -fPIC -Wall -Wextra -O2 $(DLL_FLAGS)
CXXFLAGS := -std=c++11 -fPIC -Wall -Wextra -O2 $(DLL_FLAGS)
LDFLAGS  := -shared

# Detect OS for extension
ifeq ($(OS),Windows_NT)
    TARGET := libctool.dll
    LIBS   := -lws2_32
else
    TARGET := libctool.so
    LIBS   := -lpthread
endif

# Directories
CB_DIR := cb
CT_DIR := ct
BIN_DIR := bin

# Source and Object files
CB_SRCS := $(wildcard $(CB_DIR)/*.c) CBridge.c
CT_SRCS := $(wildcard $(CT_DIR)/*.cpp) CTool.cpp

CB_OBJS := $(CB_SRCS:%.c=$(BIN_DIR)/%.o)
CT_OBJS := $(CT_SRCS:%.cpp=$(BIN_DIR)/%.o)

# Default rule
all: $(BIN_DIR) $(TARGET)

# Create bin directory
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BIN_DIR)/$(CB_DIR)
	@mkdir -p $(BIN_DIR)/$(CT_DIR)

# Link the shared library
$(TARGET): $(CB_OBJS) $(CT_OBJS)
	@echo "Linking $@..."
	@$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo "Build successful."

# Compile C source files (CBridge)
$(BIN_DIR)/%.o: %.c
	@echo "Compiling C: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files (CTool)
$(BIN_DIR)/%.o: %.cpp
	@echo "Compiling C++: $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup
clean:
	@echo "Cleaning up..."
	@rm -rf $(BIN_DIR) $(TARGET)

.PHONY: all clean