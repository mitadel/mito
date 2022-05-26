all: main

main: main.o
	g++ -o $@ $^

main.o: main.cc
	g++ -c $< -std=c++20

output:
	./main

clean:
	rm -f *.o main
