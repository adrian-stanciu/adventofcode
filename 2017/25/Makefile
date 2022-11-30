.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++20 -O3 -Wall

build: main
main: main.o
main.o: main.cpp

run: main
	./main < input

clean:
	rm -f *.o main

