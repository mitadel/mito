#include <iostream>
#include <map>
#include <yaml-cpp/yaml.h>

typedef double real;

class YAMLInputFile {

  // public methods
  public:
    // constructor from string with input file name 
    YAMLInputFile(std::string inputFileName); 
    // constructor from command line arguments "--input input.dat"
    YAMLInputFile(int argc, char** argv); 
    // printout values
    void Display() const;

    // retrieve number stored in dictionary through its key
    inline real GetReal(std::string key) const {return _file[key].as<real>();}
    // retrieve string stored in dictionary through its key
    inline std::string GetString(std::string key) const {return _file[key].as<std::string>();}

  // private methods
  private:
    int _readFile(std::string inputFileName);

  // private attributes
  public:
    YAML::Node _file;

};