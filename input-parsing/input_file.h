#include <iostream>
#include <map>

using real = double;

class InputFile {

    // public methods
public:
    // constructor from string with input file name
    InputFile(std::string inputFileName);
    // constructor from command line arguments "--input input.dat"
    InputFile(int argc, char ** argv);
    // printout values
    void Display() const;

    // retrieve number stored in dictionary through its key
    inline real GetReal(std::string key) const { return _dictionaryReals.at(key); }
    // retrieve string stored in dictionary through its key
    inline std::string GetString(std::string key) const { return _dictionaryStrings.at(key); }

    // private methods
private:
    int _readFile(std::string inputFileName);

    // private attributes
private:
    std::map<std::string, real> _dictionaryReals;
    std::map<std::string, std::string> _dictionaryStrings;
};