CC=g++ 
FLAGS=-Wall -O3

default: prepare opensidescan-gui
	
opensidescan-gui:
	cd build/bin && qmake ../../src/OpenSidescan/.
	cd build/bin && make

prepare:
	mkdir -p build
	mkdir -p build/bin

clean:
	rm -rf build
