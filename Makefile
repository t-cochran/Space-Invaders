#  include directory
IDIR =../include

# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
CFLAGS  = -c -g -Wall -I $(IDIR)

#  SFML libraries
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

# the build target executable:
TARGET = main

all:
	$(CC) $(CFLAGS) $(TARGET).cpp
	$(CC) $(TARGET).o -o $(TARGET) $(LIBS)
	$(CC) -c -g -Wall tests.cpp 
	$(CC) tests.o -o tests

run:
	./$(TARGET)

test:
	@./tests

clean:
	rm -f $(TARGET) tests helpers *.o *~ 