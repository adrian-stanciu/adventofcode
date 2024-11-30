.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++23 -O2 -Wall -Wextra -I../common

build: main
main: main.o
main.o: main.cpp ../common/asm_computer.hpp

run: main
	./main < input

clean:
	rm -f *.o main

