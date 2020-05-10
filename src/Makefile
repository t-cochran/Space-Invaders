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
	$(CC) $(CFLAGS) soundClass.cpp
	$(CC) $(CFLAGS) util.cpp
	$(CC) $(TARGET).o util.o entityClass.o soundClass.o -o $(TARGET) $(LIBS)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o *~ 