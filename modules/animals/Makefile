all: main

main: main.o animal.pcm animal.o cat.pcm
	clang++ -std=c++2a -fprebuilt-module-path=. animal.o main.o -o main

main.o: main.cpp animal.pcm cat.pcm
	clang++ -std=c++2a -fprebuilt-module-path=. -c main.cpp -o main.o

animal.o: animal.cpp animal.pcm
	clang++ -std=c++2a -c animal.cpp -fmodule-file=animal.pcm -o animal.o 

animal.pcm: animal.cpp
	clang++ -std=c++2a --precompile animal.cppm -o animal.pcm

cat.pcm: cat.cpp animal.pcm
	clang++ -std=c++2a -fprebuilt-module-path=. -c cat.cpp -Xclang -emit-module-interface -o cat.pcm

clean: 
	rm main main.o animal.pcm animal.o cat.pcm