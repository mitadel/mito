all: gent

gent: gent.o
	g++ -o $@ $^

gent.o: gent.cc
	g++ -c $<

output:
	./gent

clean:
	rm -f *.o gent
