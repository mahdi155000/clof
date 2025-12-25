# Compiler and flags
CC := gcc
CFLAGS := -Wall -g -pthread -I. -Iplugins -Isrc
LDFLAGS := -lncursesw -lsqlite3 -lcurl -lcjson -pthread

# Directories
SRC_DIR := src
PLUGIN_DIR := plugins

# Automatically collect all .c files
SRC_FILES := $(wildcard *.c) $(wildcard $(SRC_DIR)/*.c) $(wildcard $(PLUGIN_DIR)/*.c)
OBJ_FILES := $(SRC_FILES:.c=.o)

# Target executable
TARGET := clof

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean objects and executable
clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Phony targets
.PHONY: all clean
