.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++23 -O2 -Wall -Wextra

build: main
main: main.o
main.o: main.cpp

run: main
	./main < input

clean:
	rm -f *.o main

