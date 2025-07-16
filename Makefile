# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fPIC
LDFLAGS = -ldl

# Add -rdynamic for main executable linking
LDFLAGS_EXE = $(LDFLAGS) -rdynamic

# Target executable
TARGET = clof.out

# Core source files
CORE_SRCS = main.c storage.c plugins_manager.c
CORE_OBJS = $(CORE_SRCS:.c=.o)

# Find plugin source directories
PLUGIN_DIRS := $(wildcard plugins/*)
PLUGIN_SO := $(addsuffix /plugin.so, $(PLUGIN_DIRS))

# Default target
all: $(TARGET) plugins

# Link final executable with -rdynamic
$(TARGET): $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS_EXE)

# Generic rule to build .o files from .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build all plugins as .so
plugins: $(PLUGIN_SO)

# Compile each plugin .c to plugin.so inside its folder
plugins/%/plugin.so: plugins/%/*.c plugins_manager.h
	$(CC) $(CFLAGS) -shared -o $@ $<

# Clean build artifacts
clean:
	rm -f $(CORE_OBJS) $(TARGET)
	find plugins -name '*.so' -delete
