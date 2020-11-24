#include <cmath>
#include "../math.h"

mito::real cos(const mito::vector<mito::DIM2>& x)
{
    return cos(x[0] * x[1]); 
}

int main () {

    std::array<mito::function<mito::vector<mito::DIM2>, mito::real>, mito::DIM2> Df;

    mito::function<mito::vector<mito::DIM2>, mito::real > Dx{ 
        [](const mito::vector<mito::DIM2>& x){ return -sin(x[0] * x[1])* x[1]; } };

    mito::function<mito::vector<mito::DIM2>, mito::real > Dy{ 
        [](const mito::vector<mito::DIM2>& x){ return -sin(x[0] * x[1])* x[0]; } };

    Df[0] = Dx;
    Df[1] = Dy;

    mito::Function<mito::DIM2> cosFunction(cos, Df);

    mito::vector<mito::DIM2> X = {0.0, 0.0};
    std::cout << cosFunction(X) << std::endl; 
    std::cout << cosFunction.Grad(X) << std::endl; 
    std::cout << cosFunction.Div(X) << std::endl; 


    return 0;
}