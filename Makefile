CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

clof: main.c
	$(CC) $(CFLAGS) main.c -o clof

clean:
	rm -f clof

