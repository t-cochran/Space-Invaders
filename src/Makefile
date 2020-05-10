#  include directory
IDIR =../include

# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
CFLAGS  = -c -g -Wall -I$(IDIR)

#  SFML libraries
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

# the build target executable:
TARGET = main

all:
	$(CC) $(CFLAGS) $(TARGET).cpp
	$(CC) $(CFLAGS) entityClass.cpp
	$(CC) $(CFLAGS) helpers.cpp
	$(CC) $(TARGET).o helpers.o entityClass.o -o $(TARGET) $(LIBS)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET) tests helpers *.o *~ 