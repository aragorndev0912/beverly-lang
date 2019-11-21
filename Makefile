NAME=beverly
CC=gcc
CFLAGS=-Wall -g

main: main.c
	$(CC) $(CFLAGS) -o $(NAME) main.c -std=c11

.PHONY: clean
clean:
	del /s *.o