CC=g++

all: my_prog

my_prog: main.o square.o 
	$(CC) -o my_prog main.o square.o

main.o: main.cpp
	$(CC) -O2 -c main.cpp

square.o: square.cpp square.h
	$(CC) -O2 -c square.cpp

clean:
	rm -rf *.o my_prog