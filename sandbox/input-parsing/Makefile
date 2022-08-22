all: parse

parse: parse.o yaml_input_file.o input_file.o
	g++ -o $@ $^ -lyaml-cpp -L/usr/local/yaml/lib

yaml_input_file.o: yaml_input_file.cc
	g++ -c -std=c++17 $< -I/usr/local/yaml/include

input_file.o: input_file.cc
	g++ -c -std=c++17 $<

parse.o: parse.cc
	g++ -c $< -I/usr/local/yaml/include

output:
	./parse

clean:
	rm -f *.o parse