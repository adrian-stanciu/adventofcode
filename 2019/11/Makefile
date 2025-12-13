.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++23 -O2 -Wall -Wextra -I../common -I../../common

build: sol
sol: sol.o
sol.o: sol.cpp ../common/int_computer.hpp ../../common/hasher.hpp

run: sol
	./sol < input

clean:
	rm -f *.o sol

