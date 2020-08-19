CC=g++
FLAGS=
INCLUDES=

build_exec_dir=build/bin
test_exec_dir=build/test/bin
test_result_dir=build/reports

test-file-lock:
	mkdir -p $(test_exec_dir)
	mkdir -p $(test_result_dir)
	$(CC) $(FLAGS) $(INCLUDES) -o $(test_exec_dir)/lockTests locker.cpp

locker: 
	mkdir -p $(build_exec_dir)
	$(CC) $(FLAGS) $(INCLUDES) -o build/bin/locker locker.cpp
clean:
	rm -rf build


.PHONY: 
