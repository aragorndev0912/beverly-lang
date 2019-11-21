NAME=beverly
CC=gcc
CFLAGS=-Wall -g

main: main.c object.o
	$(CC) $(CFLAGS) -o $(NAME) main.c object.o -std=c11

object.o: src/object.c includes/object.h
	$(CC) $(CFLAGS) -c src/object.c

.PHONY:clean all
clean:
	del /s *.o
all:
	del /s *.o
	del *.exe