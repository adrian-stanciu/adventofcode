.PHONY: build run clean

CC=g++
CXXFLAGS=-std=c++20 -O3 -Wall -I../../common
LDLIBS=-lcrypto

build: main
main: main.o
main.o: main.cpp ../../common/md5.h

run: main
	./main < input

clean:
	rm -f *.o main

