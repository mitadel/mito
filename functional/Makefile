all: functional

functional: functional.o
	g++ -o $@ $^

functional.o: functional.cc
	g++ -c $<

output:
	./functional

clean:
	rm -f *.o functional
