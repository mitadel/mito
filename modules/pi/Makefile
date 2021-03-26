all: main

main: pi.pcm
	clang++ -o main -std=c++2a -fprebuilt-module-path=. main.cpp pi.cpp

pi.pcm: pi.cpp
	clang++ -std=c++2a -c pi.cpp -Xclang -emit-module-interface -o pi.pcm

clean: 
	rm main pi.pcm