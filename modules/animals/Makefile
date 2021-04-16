all: main

main: animal.pcm cat.pcm
	clang++ -o main -std=c++2a -fprebuilt-module-path=. main.cpp animal.cpp cat.cpp

animal.pcm: animal.cpp
	clang++ -std=c++2a -c animal.cpp -Xclang -emit-module-interface -o animal.pcm

cat.pcm: cat.cpp animal.pcm
	clang++ -std=c++2a -c -fprebuilt-module-path=. cat.cpp -Xclang -emit-module-interface -o cat.pcm

clean: 
	rm main animal.pcm cat.pcm