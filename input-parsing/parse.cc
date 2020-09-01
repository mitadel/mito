#include "input_file.h"

static const std::string inputFileName = "input.dat";

int main (int argc, char** argv) {

    //InputFile input(inputFileName);
    InputFile input(argc, argv);
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