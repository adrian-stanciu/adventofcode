.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++20 -O2 -Wall -Wextra

build: main
main: main.o
main.o: main.cpp

run: main
	./main < input

clean:
	rm -f *.o main

