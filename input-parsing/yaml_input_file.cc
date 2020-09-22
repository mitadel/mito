#include "yaml_input_file.h"
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/node/node.h>

YAMLInputFile::YAMLInputFile(std::string inputFileName) 
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

YAMLInputFile::YAMLInputFile(int argc, char** argv) 
    : YAMLInputFile(getFileName(argc, argv)) {}

int YAMLInputFile::_readFile(std::string inputFileName) 
{
    _file = YAML::LoadFile(inputFileName);

    // all done
    return 0;
}

void YAMLInputFile::Display() const {

    // report
    std::cout << "The following input parameters were read:" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    
    // loop on file 
    for (YAML::const_iterator it=_file.begin(); it!=_file.end(); ++it) {
        try {
            it->second.as<real>();
        }
        catch (...)
        {
            try {
                it->second.as<std::string>();
            }
            catch (...)
            {
                std::cout << "Error!" << std::endl;
            }
            
            // report
            std::cout << "\t" << it->first.as<std::string>() << ":\t\t" 
                << it->second.as<std::string>() 
                << std::endl;
            
            // move on
            continue;
        }

        // report
        std::cout << "\t" << it->first.as<std::string>() << ":\t\t" 
            << it->second.as<real>() 
            << std::endl;
    }

    // report
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    // all done
    return;
}