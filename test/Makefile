# Makefile for fact.c with big.c and big.h

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Include directory for header files
INC_DIR = .

# Library directory
LIB_DIR = .

# Libraries to link
LIBS = -lm

# Target executable
TARGET = fact

# Source files
SRCS = fact.c big.c

# Object files
OBJS = $(SRCS:.c=.o)

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -L$(LIB_DIR) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INC_DIR)

clean:
	$(RM) $(TARGET) $(OBJS)

