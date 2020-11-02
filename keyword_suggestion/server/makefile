CC := g++
RM := rm -rf
SRCS := $(wildcard src/*.cc)
BINS := bin/server
$(BINS): $(SRCS)
	$(CC) $^ -o $@ -lpthread
.PHONY: clean
clean:
	$(RM) $(BINS)
