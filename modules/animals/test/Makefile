all: 
	mkdir -p build
	make build/main

build/main: build/main.o
	clang++ -std=c++2a -fprebuilt-module-path=../extension/build build/main.o ../lib/build/animal.o ../extension/build/cat.o -o build/main

build/main.o: main.cpp
	clang++ -std=c++2a -fprebuilt-module-path=../extension/build -I/usr/local/pyre/include -c main.cpp -o build/main.o

check:
	./build/main

clean: 
	rm -r build
