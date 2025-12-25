CC = gcc
CFLAGS = -Wall -g -pthread

SRC = \
	main.c \
	plugins/tui.c \
	movie.c \
	db.c\
	plugins/plugins.c \
	plugins/lof.c \
	plugins/list.c \
	plugins/add.c \
	plugins/update.c \
	plugins/remove.c \
	plugins/help.c \
	plugins/search.c\
	plugins/reid.c \
	src/vlc_tracker.c\
	src/vlc_tracker_thread.c


OBJ = $(SRC:.c=.o)

TARGET = clof

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lncursesw -lsqlite3 -lcurl -lcjson -pthread

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
