#Makefile for Assignment 4 : Multi-Process Downloader

# Compiler
CC = gcc
# Flags
CFLAGS = -Wall -std=gnu99 -pedantic
# Source files
SRCS = $(wildcard *.c)
# Object files
OBJS = $(SRCS:.c=.o)
# Arguments
ARGS = test_urls.txt 1
# Executable name
EXEC = a4download



# Default target
all: $(EXEC)

# Executable target
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean target
clean:
	rm -f $(EXEC) $(OBJS)

clearexec:
	rm -f $(EXEC)

# Run target
run: $(EXEC)
	./$(EXEC) $(ARGS)

# Debug: Run all then run run
debug: all run