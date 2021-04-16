CXX = /usr/local/gcc11/bin/g++

all: main

main: main.o pi.o pi-i.o
	$(CXX) -o main main.o pi.o pi-i.o

main.o: main.cpp pi.o pi-i.o
	$(CXX) -fmodules-ts -std=c++20 -c main.cpp

pi.o: pi.cpp pi-i.o
	$(CXX) -fmodules-ts -std=c++20 -c pi.cpp

pi-i.o: pi-i.cpp
	$(CXX) -fmodules-ts -std=c++20 -c pi-i.cpp

clean: 
	rm -r gcm.cache main main.o pi.o pi-i.o