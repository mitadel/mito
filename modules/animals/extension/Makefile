all: 
	mkdir -p build
	make build/cat.o

build/cat.o: cat.cpp build/cat.pcm
	clang++ -std=c++2a -c cat.cpp -fmodule-file=build/cat.pcm -o build/cat.o 

build/cat.pcm: cat.cpp
	clang++ -std=c++2a -fprebuilt-module-path=../lib/build --precompile cat.cppm -o build/cat.pcm

clean: 
	rm -r build