CC=g++ 
FLAGS=-Wall -O3


test_exec_dir=build/test/bin

default: prepare opensidescan-gui
	
opensidescan-gui:
	cd build/bin && qmake -DENABLE_PRECOMPILED_HEADERS=OFF ../../src/OpenSidescan/.
	cd build/bin && make

testGUI:
	mkdir -p $(test_exec_dir)
	cd $(test_exec_dir) && qmake CONFIG+=debug ../../../test/testGUI/.
	cd $(test_exec_dir) && make
	cd $(test_exec_dir) && ./testGUI -o test-report-OpenSidescanXUNIT.xml,xunitxml -o test-report-OpenSidescan.xml,xml -o test-report-OpenSidescanTAP.txt,tap


prepare:
	mkdir -p build
	mkdir -p build/bin

clean:
	rm -rf build

.PHONY: testGUI
