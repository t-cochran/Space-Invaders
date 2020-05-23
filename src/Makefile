#  Header file directory
IDIR =../include

# GNU g++ Compiler
CC = g++

# Compiler Flags
CFLAGS  = -c -g -O -Wall -I$(IDIR)

#  Linked SFML libraries
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

# Build main:
all: main.o
	$(CC) main.o util.o entities.o sounds.o projectiles.o -o main $(LIBS)

main.o: main.cpp util.o
	$(CC) $(CFLAGS) main.cpp

util.o: util.cpp entities.o sounds.o projectiles.o
	$(CC) $(CFLAGS) util.cpp

projectiles.o: projectiles.cpp
	$(CC) $(CFLAGS) projectiles.cpp

entities.o: entities.cpp
	$(CC) $(CFLAGS) entities.cpp 

sounds.o: sounds.cpp
	$(CC) $(CFLAGS) sounds.cpp 

#  Run game
run:
	./main

#  Run main through valgrind
memcheck:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file="leaks.txt" ./main run

#  Cleanup
clean:
	rm -f main *.o *~ 
