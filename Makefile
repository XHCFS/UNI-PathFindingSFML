# Makefile for compiling SFML project on Linux and Windows

# Compiler
CC := g++
# Compiler flags
CFLAGS := -std=c++11 -Wall -Wextra -I./src/include
# Linker flags
LDFLAGS := 

# SFML libraries
SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Source files
SRCS := $(wildcard *.cpp)
# Object files
OBJS := $(SRCS:.cpp=.o)

# Output executable
EXEC := app

# Compilation rule for Linux
linux: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(SFML_LIBS) -o $(EXEC)

# Compilation rule for Windows
windows: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(SFML_LIBS) -o $(EXEC).exe

# Generic compilation rule for object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(EXEC) $(EXEC).exe

