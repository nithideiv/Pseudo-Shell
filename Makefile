# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Executable name
TARGET = pseudo-shell

# Source files and object files
SRCS = main.c command.c string_parser.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Compile source files into object files
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

command.o: command.c command.h
	$(CC) $(CFLAGS) -c command.c -o command.o 

string_parser.o: string_parser.c string_parser.h
	$(CC) $(CFLAGS) -c string_parser.c

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
