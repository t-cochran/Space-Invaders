#  include directory
IDIR =../include

# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
CFLAGS  = -c -g -O -Wall -I$(IDIR)

#  SFML libraries
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

# the build target executable:
TARGET = main

all:
	$(CC) $(CFLAGS) $(TARGET).cpp
	$(CC) $(CFLAGS) entityClass.cpp
	$(CC) $(CFLAGS) soundClass.cpp
	$(CC) $(CFLAGS) projectileClass.cpp
	$(CC) $(CFLAGS) util.cpp
	$(CC) $(TARGET).o util.o entityClass.o soundClass.o projectileClass.o -o $(TARGET) $(LIBS)

run:
	./$(TARGET)

leak:
	valgrind --leak-check=full ./$(TARGET) run

clean:
	rm -f $(TARGET) *.o *~ 