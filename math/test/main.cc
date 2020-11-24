#include <cmath>
#include "../math.h"

using mito::function;
using mito::vector;
using mito::real;
using mito::DIM2;

int main () {

    // a scalar function 
    function<vector<DIM2>, real > f{ 
        [](const vector<DIM2>& x){ return cos(x[0] * x[1]); } };

    // df/dx[0]
    function<vector<DIM2>, real > Dx{ 
        [](const vector<DIM2>& x){ return -sin(x[0] * x[1])* x[1]; } };

    // df/dx[1] 
    function<vector<DIM2>, real > Dy{ 
        [](const vector<DIM2>& x){ return -sin(x[0] * x[1])* x[0]; } };

    // its partial derivatives 
    std::array<function<vector<DIM2>, real>, DIM2> Df = {Dx, Dy};

    // instantiate a scalar function object
    mito::ScalarField<DIM2> cosine(f, Df);

    // a point in the reference configuration
    vector<DIM2> X = {0.0, 0.0};

    // evaluate f and its gradient
    std::cout << "Evaluating cosine function cos(x[0] * x[1]) at X = " << X << " : " << 
        cosine(X) << std::endl; 
    std::cout << "Evaluating gradient of cosine function at X = " << X << " : " << 
        mito::Grad<DIM2>(cosine, X) << std::endl; 

    // instantiate a vector function object
    mito::VectorField<2, DIM2> cosineVector({cosine, cosine});

    // evaluate divergence
    std::cout << "Evaluating divergence of cosine vector function at X = " << X << " : " << 
        mito::Div<DIM2>(cosineVector, X) << std::endl; 

    // build vector field with gradient of cosine function 
    mito::VectorField<2, DIM2> gradient = mito::Grad<DIM2>(cosine);
    std::cout << "Evaluating gradient of cosine function at X = " << X << " : " << 
        gradient(X) << std::endl; 

    return 0;
}

// end of file
