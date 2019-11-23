NAME=beverly
CC=gcc
CFLAGS=-Wall -g

main: main.c 
	$(CC) $(CFLAGS) -o $(NAME) main.c -std=c11

.PHONY:clean all
clean:
	del /s *.o
all:
	del /s *.o
	del *.exe