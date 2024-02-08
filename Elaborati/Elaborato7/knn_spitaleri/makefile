SOURCES := src

CC := gcc
CFLAGS := -Wall -lm # -lm needed for math.h

EXEC := main.out

all: build run

build:
	$(CC) $(SOURCES)/*.c -o $(EXEC) $(CFLAGS)

run:
	./$(EXEC)