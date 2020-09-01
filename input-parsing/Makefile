all: parse

parse: parse.o input_file.o
	g++ -o $@ $^

input_file.o: input_file.cc
	g++ -c -std=c++17 $<

parse.o: parse.cc
	g++ -c $<

output:
	./parse

clean:
	rm -f *.o parse
