.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++23 -O2 -Wall -Wextra -I../../common
LDLIBS=-lmbedcrypto

build: sol
sol: sol.o
sol.o: sol.cpp ../../common/md5.hpp

run: sol
	./sol < input

clean:
	rm -f *.o sol

