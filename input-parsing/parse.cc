#define WITH_YAML

#ifdef WITH_YAML
#include "yaml_input_file.h"
static const std::string inputFileName = "input.yaml";
#else
#include "input_file.h"
static const std::string inputFileName = "input.dat";
#endif

int main (int argc, char** argv) {

#ifdef WITH_YAML
    YAMLInputFile input(inputFileName);
    //YAMLInputFile input(argc, argv);
#else
    InputFile input(inputFileName);
    //InputFile input(argc, argv);
#endif
    input.Display();

    real nu             = input.GetReal("Poisson's ratio");
    std::string output  = input.GetString("output directory");
    real Ka             = input.GetReal("bulk modulus a");
    real Kb             = input.GetReal("bulk modulus b");
    int Nmax            = input.GetReal("Nmax");

    std::cout << "Poisson's ratio\t" << nu << std::endl;
    std::cout << "output\t" << output << std::endl;
    std::cout << "Ka\t" << Ka << std::endl;
    std::cout << "Kb\t" << Kb << std::endl;
    std::cout << "Nmax\t" << Nmax << std::endl;

    return 0;
}