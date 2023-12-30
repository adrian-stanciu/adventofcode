.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++20 -O2 -Wall -Wextra -I../common

build: main
main: main.o
main.o: main.cpp ../common/int_computer.hpp

run: main
	./main < input

clean:
	rm -f *.o main

