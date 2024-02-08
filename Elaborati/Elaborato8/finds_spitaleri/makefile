SOURCES := src

CC := gcc
CFLAGS := -Wall -lm # -lm needed for math.h

EXEC := main.out
DATASET := dataset.csv

build:
	$(CC) $(SOURCES)/*.c -o $(EXEC) $(CFLAGS)

file: build
	./$(EXEC) file $(DATASET)

console: build
	./$(EXEC) console