.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++20 -O3 -Wall -I../common -I../../common

build: main
main: main.o
main.o: main.cpp ../common/int_computer.h ../../common/hasher.h

run: main
	./main < input

clean:
	rm -f *.o main

