.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++17 -O3 -Wall -I../../common

build: main
main: main.o
main.o: main.cpp ../../common/hasher.h

run: main
	./main < input

clean:
	rm -f *.o main

