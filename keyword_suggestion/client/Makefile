CC = g++

RM = rm -rf

SRCS = $(wildcard src/*.cc)

BIN = bin/client

FLAGS = -g -Wall -lpthread

$(BIN): $(SRCS)
	$(CC) $^ -o $@ $(FLAGS)

.PHONY: clean
clean:
	$(RM) $(BINS)
	