CC=g++ 
FLAGS=-Wall -O3
FILES= src/OpenSidescan/sidescanimager.cpp src/OpenSidescan/sidescanimage.cpp src/OpenSidescan/georeferencedobject.cpp src/OpenSidescan/sidescanfile.cpp src/OpenSidescan/opencvhelper.cpp src/thirdParty/MBES-lib/src/datagrams/DatagramParser.cpp src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.cpp src/thirdParty/MBES-lib/src/datagrams/s7k/S7kParser.cpp src/thirdParty/MBES-lib/src/datagrams/kongsberg/KongsbergParser.cpp src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.cpp src/thirdParty/MBES-lib/src/utils/NmeaUtils.cpp src/thirdParty/MBES-lib/src/utils/StringUtils.cpp src/thirdParty/MBES-lib/src/sidescan/SidescanPing.cpp

default: prepare opensidescan-gui cli
	
opensidescan-gui:
	cd build/bin && qmake ../../src/OpenSidescan/.
	cd build/bin && make

cli: prepare detect dump
	echo "Building command line utilities..."

detect:
	$(CC) `pkg-config --cflags --libs opencv eigen3` -o build/bin/sidescan-dump src/cli/sidescan-dump.cpp $(FILES)

dump:
	$(CC) `pkg-config --cflags --libs opencv eigen3` -o build/bin/sidescan-detect src/cli/sidescan-detect.cpp $(FILES)

fit: prepare
	$(CC) `pkg-config --cflags --libs opencv eigen3` -o build/bin/fit-detector src/cli/fit-detector.cpp $(FILES)

prepare:
	rm -rf build || true
	mkdir -p build
	mkdir -p build/bin

clean:
	rm -rf build
