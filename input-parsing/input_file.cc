#include "input_file.h"
#include <fstream>
#include <regex>
#include <cassert>
#include <algorithm>

/* helper functions to trim string */
static constexpr std::string WHITESPACE = " \n\r\t\f\v";
std::string ltrim(const std::string& s)
{
	int start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}
std::string rtrim(const std::string& s)
{
	int end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
std::string trim(const std::string& s)
{
	return rtrim(ltrim(s));
}

InputFile::InputFile(std::string inputFileName) 
    : _dictionaryReals(), _dictionaryStrings()
{
    if (inputFileName == "") return;

    // report
    std::cout << "Reading file " << inputFileName << std::endl;

    // read input file 
    _readFile(inputFileName);

    // all done
    return;
} 

/* helper function to get input file name from command line arguments */
namespace {
std::string getFileName(int argc, char** argv)
{
    // default input filename
    std::string inputFileName = "input.dat";
    // loop on command line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--input") {
            // make sure we aren't at the end of argv
            if (i + 1 < argc) { 
                inputFileName = argv[++i];
            } 
            // uh-oh, there was no argument to the destination option
            else { 
                // report error
                std::cout << "Error: --input option requires one argument." << std::endl;
                std::cout << "File not read..." << std::endl;
                return "";
            }  
        }
    }

    return inputFileName;
}
}

InputFile::InputFile(int argc, char** argv) 
    : InputFile(getFileName(argc, argv)) {}

void InputFile::Display() const
{
    // report
    std::cout << "The following input parameters were read:" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "Numeric values:" << std::endl;
    // display real values 
    for(auto const& [key, value] : _dictionaryReals)
    {
        std::cout << "\t" << key << ":" << "\t\t" << value << std::endl;
    }

    // report
    std::cout << "String values:" << std::endl;
    // display string values 
    for(auto const& [key, value] : _dictionaryStrings)
    {
        std::cout << "\t" << key << ":" << "\t\t" << value << std::endl;
    }

    // report
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    // all done
    return;
}

int InputFile::_readFile(std::string inputFileName) 
{
    // try to open file
    std::ifstream inputFile(inputFileName.data());
    // if it fails
    if (inputFile.fail()) {
        // report error
        std::cout << "Error: Cannot open input file: " << inputFileName << std::endl;
        // all done
        return 1;
    }

    // string to store the current line 
    std::string line;

    // regex to match a line of the following form:
    //      a number or a string                (the value)             followed by
    //      at least one space                                          followed by
    //      any non-# and non-white character   (the key)               followed by
    //      optionally a # followed by any character (comment)
    // 
    // match space zero or more times and a string of characters only
    //std::regex e ("^[\\s]*([A-Za-z]+)"); 
    // match space zero or more times and a string of characters followed by anything else
    //std::regex e ("^[\\s]*([A-Za-z]+)(.*)"); 
    // match space zero or more times and a string of characters followed by a space and then 
    // anything else
    //std::regex e ("^[\\s]*([A-Za-z]+)[\\s]+(.*)");
    // match: 
    //      space zero or more times    [\\s]*          followed by
    //      a string of characters      ([A-Za-z]+)     followed by 
    //      at least one white space    [\\s]+          followed by
    //      any character but endline   (.+)            followed by
    //      optionally a # sign         (\x23*)         followed by 
    //      anything else               (.*)
    //std::regex e ("^[\\s]*([A-Za-z]+)[\\s]+(.+)(\x23*)(.*)");
    std::string initiation("^[\\s]*");
    // problems with tabs after value being interpreted as key:
    //std::string termination("[\\s]+([^\x23]+)(?:\x23*)(?:.*)"); 
    std::string termination("[\\s]+([A-Za-z0-9-_]+[^\x23]*)(?:.*)");
    std::string string_expression("[^0-9-](?:[A-Za-z0-9-_]+)");
    // regex to match a string value 
    std::regex string_regex (initiation+"("+string_expression+")"+termination);
    std::string e_number_expression("(?:(?:e|E)[+|-]?[0-9]+)");
    std::string number_expression("-?(?:[0-9]+|[0-9]*.[0-9]+|[0-9]+.[0-9]*)"
        +e_number_expression+"?"); 
    // regex to match a numeric value 
    std::regex number_regex(initiation+"("+number_expression+")"+termination);
    std::smatch match;

    // while there are newlines to be read
    while (std::getline(inputFile, line))
    {
        //std::cout << "Line: " << line << std::endl;
        bool isString = false;
        bool isNumber = false;

        // if the line starts with a string (string parameter)
        if (std::regex_match(line, match, string_regex)) {
            // record it is a string
            isString = true;
            // assert you were able to find (match) a value and a key 
            assert(match.size()==3);
            // second group matched is the key
            std::string key = trim(match[2].str());
            // first group matched is the value
            std::string value = trim(match[1].str());
            // debug output
            //std::cout << "string: " << std::endl;
            //std::cout << "\tvalue = " << value << std::endl;
            //std::cout << "\tkey = " << key << std::endl;
            //std::cout << std::endl;
            // store in dictionary (this overwrites an existing pair with the same key)
            _dictionaryStrings[key] = value;
        }

        // if the line starts with a number (numeric parameter)
        if (std::regex_match(line, match, number_regex)) {
            // record it is a number
            isNumber = true;
            // assert you were able to find (match) a value and a key 
            assert(match.size()==3);
            // second group matched is the key
            std::string key = trim(match[2].str());
            // first group matched is the value
            std::string value = trim(match[1].str());
            // fill up a real with the numeric value of input
            std::stringstream number_stream(value); 
            real number = 0; number_stream >> number;
            // debug output
            //std::cout << "number : " << std::endl;
            //std::cout << "\tvalue = " << number << std::endl;
            //std::cout << "\tkey = " << key << std::endl;
            //std::cout << std::endl;
            // store in dictionary (this overwrites an existing pair with the same key)
            _dictionaryReals[key] = number;
        }

        // if the line starts with an expression that can be interpreted either as a 
        //  string or as a number
        if (isString && isNumber) 
        {
            // report warning
            std::cout << "Warning: The following line might be either a string or a number..." 
                << std::endl; 
            std::cout << line << std::endl;
            std::cout << std::endl;   
        }

        // if the line starts with an expression that cannot be interpreted either as a 
        //  string or as a number
        if (!isString && !isNumber) {

            // report warning
            std::cout << "Warning: Cannot read line: " << std::endl; 
            std::cout << line << std::endl;
            std::cout << std::endl;
        }
    }

    // all done
    return 0;
}