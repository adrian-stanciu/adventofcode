.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++17 -O3 -I../int_computer

build: main
main: main.o
main.o: main.cpp ../int_computer/int_computer.h

run: main
	./main < input

clean:
	rm -f *.o main

