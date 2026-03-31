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

CFLAGS   := -std=c99 -fPIC -Wall -Wextra -O2 $(DLL_FLAGS) -MMD -MP
CXXFLAGS := -std=c++11 -fPIC -Wall -Wextra -O2 $(DLL_FLAGS) -MMD -MP
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
CB_SRCS := $(shell find $(CB_DIR) -name "*.c") CBridge.c
CT_SRCS := $(shell find $(CT_DIR) -name "*.cpp") CTool.cpp

CB_OBJS := $(CB_SRCS:%.c=$(BIN_DIR)/%.o)
CT_OBJS := $(CT_SRCS:%.cpp=$(BIN_DIR)/%.o)

# Dependency files tracking headers
DEPS := $(CB_OBJS:.o=.d) $(CT_OBJS:.o=.d)

# Default rule
all: $(BIN_DIR) $(TARGET)

# Create bin directory
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Link the shared library
$(TARGET): $(CB_OBJS) $(CT_OBJS)
	@echo "Linking $@..."
	@$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo "Build successful."

# Compile C source files (CBridge)
$(BIN_DIR)/%.o: %.c
	@echo "Compiling C: $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files (CTool)
$(BIN_DIR)/%.o: %.cpp
	@echo "Compiling C++: $<"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files so make knows about header changes
-include $(DEPS)

# Cleanup
clean:
	@echo "Cleaning up..."
	@rm -rf $(BIN_DIR) $(TARGET)

.PHONY: all clean