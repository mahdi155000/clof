CC = gcc
CFLAGS = -Wall -g

SRC = \
	main.c \
	movie.c \
	plugins/plugins.c \
	plugins/list.c \
	plugins/add.c \
	plugins/update.c \
	plugins/remove.c \
	plugins/help.c

OBJ = $(SRC:.c=.o)

TARGET = clof

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
