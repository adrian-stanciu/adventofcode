.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++23 -O2 -Wall -Wextra

build: sol
sol: sol.o
sol.o: sol.cpp

run: sol
	./sol < input

clean:
	rm -f *.o sol

