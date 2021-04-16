all: main

main: main.o animal.pcm animal.o cat.pcm cat.o
	clang++ -std=c++2a -fprebuilt-module-path=. main.o animal.o cat.o -o main

main.o: main.cpp animal.pcm cat.pcm
	clang++ -std=c++2a -fprebuilt-module-path=. -c main.cpp -o main.o

animal.o: animal.cpp animal.pcm
	clang++ -std=c++2a -c animal.cpp -fmodule-file=animal.pcm -o animal.o 

animal.pcm: animal.cpp
	clang++ -std=c++2a --precompile animal.cppm -o animal.pcm

cat.o: cat.cpp cat.pcm animal.pcm
	clang++ -std=c++2a -c cat.cpp -fmodule-file=cat.pcm -o cat.o 

cat.pcm: cat.cpp
	clang++ -std=c++2a -fprebuilt-module-path=. --precompile cat.cppm -o cat.pcm

clean: 
	rm main main.o animal.pcm animal.o cat.pcm cat.o