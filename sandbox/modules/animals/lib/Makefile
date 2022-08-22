all: 
	mkdir -p build
	make build/animal.o

build/animal.o: animal.cpp build/animal.pcm
	clang++ -std=c++2a -c animal.cpp -fmodule-file=build/animal.pcm -o build/animal.o 

build/animal.pcm: animal.cpp
	clang++ -std=c++2a --precompile animal.cppm -o build/animal.pcm

clean: 
	rm -r build