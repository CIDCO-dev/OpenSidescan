CC=g++ 
FLAGS=-Wall -O3

default: prepare
	
prepare:
	mkdir -p build
	mkdir -p build/bin

clean:
	rm -rf build
