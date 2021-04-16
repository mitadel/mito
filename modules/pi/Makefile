MODULE_NAME=pi
MAIN=main

all: $(MAIN)

$(MAIN): $(MODULE_NAME).pcm $(MAIN).o $(MODULE_NAME).o
	clang++  $(MODULE_NAME).pcm $(MAIN).o $(MODULE_NAME).o -o $(MAIN)

$(MAIN).o: $(MAIN).cpp $(MODULE_NAME).pcm
	clang++ -std=c++2a -fmodules-ts -c $(MAIN).cpp -fmodule-file=$(MODULE_NAME).pcm -o $(MAIN).o

$(MODULE_NAME).o: $(MODULE_NAME).cpp $(MODULE_NAME).pcm
	clang++ -std=c++2a -fmodules-ts -c $(MODULE_NAME).cpp -fmodule-file=$(MODULE_NAME).pcm -o $(MODULE_NAME).o 

$(MODULE_NAME).pcm: $(MODULE_NAME).cpp
	clang++ -std=c++2a -fmodules-ts --precompile $(MODULE_NAME).cppm -o $(MODULE_NAME).pcm

clean: 
	rm $(MAIN) $(MODULE_NAME).pcm $(MODULE_NAME).o $(MAIN).o