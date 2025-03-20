all: clean test

clean: 
	rm -rf ./*.out

test_main: 
	/usr/bin/g++ -o ./test_main.out ./test/test_main.cpp ./test/test_test.cpp ./test/test_compiler.cpp ./src/platform/compiler/compiler.cpp

run: test_main
	./test_main.out

test: test_main
	./test_main.out
