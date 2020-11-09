CC=g++
FLAGS=-O3 -lstdc++fs
INCLUDES=-I/usr/include/eigen3 -I/usr/local/include/opencv
LIBS=-I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_dnn -lopencv_highgui -lopencv_ml -lopencv_objdetect -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_videoio -lopencv_imgcodecs -lopencv_features2d -lopencv_video -lopencv_photo -lopencv_imgproc -lopencv_flann -lopencv_core
QT_INCLUDES=-I/home/jordan/Qt5.14.1/5.14.1/gcc_64/include/QtCore -I../../Qt5.14.1/5.14.1/gcc_64/include -I../../Qt5.14.1/5.14.1/gcc_64/include/QtWidgets -I../../Qt5.14.1/5.14.1/gcc_64/include/QtGui -I../../Qt5.14.1/5.14.1/gcc_64/include/QtXml  -I. -isystem /usr/include/libdrm -I../../Qt5.14.1/5.14.1/gcc_64/mkspecs/linux-g++

FILES=src/OpenSidescan/georeferencedobject.cpp -lGL -lpthread

SS=src/OpenSidescan
SSFILES=$(SS)/sidescan/sidescanfile.cpp \
	$(SS)/sidescan/sidescanimager.cpp \
	$(SS)/sidescan/sidescanimage.cpp \
	$(SS)/inventoryobject/inventoryobject.cpp \
	$(SS)/detector/roidetector.cpp \
	$(SS)/detector/houghdetector.cpp \
	$(SS)/detector/detector.cpp \
	$(SS)/utilities/opencvhelper.cpp

THIRDPARTY=src/thirdParty
THIRDPARTYFILES=$(THIRDPARTY)/MBES-lib/src/datagrams/DatagramParser.cpp  \
		$(THIRDPARTY)/MBES-lib/src/datagrams/DatagramParserFactory.cpp  \
		$(THIRDPARTY)/MBES-lib/src/datagrams/s7k/S7kParser.cpp \
		$(THIRDPARTY)/MBES-lib/src/datagrams/kongsberg/KongsbergParser.cpp \
		$(THIRDPARTY)/MBES-lib/src/datagrams/xtf/XtfParser.cpp \
		$(THIRDPARTY)/MBES-lib/src/utils/NmeaUtils.cpp \
		$(THIRDPARTY)/MBES-lib/src/utils/StringUtils.cpp \
		$(THIRDPARTY)/MBES-lib/src/sidescan/SidescanPing.cpp \
		$(THIRDPARTY)/opencv/apps/createsamples/utility.cpp
	
LIBOPENCV=`pkg-config --cflags --libs opencv`

test_exec_dir=build/test/bin
test_result_dir=build/reports

default: prepare opensidescan-gui
	
opensidescan-gui:
	cd build/bin && qmake ../../src/OpenSidescan/.
	cd build/bin && make
	
fit-detector: prepare
	$(CC) $(FLAGS) $(INCLUDES) -o build/bin/fit-detector src/cli/fit-detector.cpp $(SSFILES) $(THIRDPARTYFILES) $(LIBOPENCV)
	
sidescan-detect: prepare
	$(CC) $(FLAGS) $(INCLUDES) -o build/bin/sidescan-detect src/cli/sidescan-detect.cpp $(FILES) $(LIBOPENCV)
	
sidescan-dump: prepare
	$(CC) $(FLAGS) $(INCLUDES) -o build/bin/sidescan-dump src/cli/sidescan-dump.cpp $(FILES) $(LIBOPENCV)
	
test: clean
	mkdir build
	cd build ; cmake ..; make ; ./tests -r console
	
lock-test:
	mkdir -p $(test_exec_dir)
	mkdir -p $(test_result_dir)
	cd test/build ; cmake ..; make ; ./lockTests -r junit -o ../../build/reports/lock-test-report.xml || true
	
unit-tests:
	mkdir -p $(test_exec_dir)
	mkdir -p $(test_result_dir)
	$(CC) $(FLAGS) $(INCLUDES) -o $(test_exec_dir)/unitTests test/main.cpp `pkg-config --cflags --libs opencv`
	$(test_exec_dir)/unitTests -r junit -o $(test_result_dir)/unit-tests-report.xml || true
	
console-unit-tests:
	mkdir -p $(test_exec_dir)
	mkdir -p $(test_result_dir)
	$(CC) $(FLAGS) $(INCLUDES) -o $(test_exec_dir)/unitTests test/main.cpp $(SSFILES) $(THIRDPARTYFILES) `pkg-config --cflags --libs opencv`
	$(test_exec_dir)/unitTests -r console

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

.PHONY: testGUI test
