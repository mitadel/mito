#include <cmath>
#include "../math.h"

int main () {

    // a function 
    mito::function<mito::vector<mito::DIM2>, mito::real > f{ 
        [](const mito::vector<mito::DIM2>& x){ return cos(x[0] * x[1]); } };

    // df/dx[0]
    mito::function<mito::vector<mito::DIM2>, mito::real > Dx{ 
        [](const mito::vector<mito::DIM2>& x){ return -sin(x[0] * x[1])* x[1]; } };

    // df/dx[1] 
    mito::function<mito::vector<mito::DIM2>, mito::real > Dy{ 
        [](const mito::vector<mito::DIM2>& x){ return -sin(x[0] * x[1])* x[0]; } };

    // its partial derivatives 
    std::array<mito::function<mito::vector<mito::DIM2>, mito::real>, mito::DIM2> Df = {Dx, Dy};

    // instantiate a scalar function object
    mito::ScalarField<mito::DIM2> cosine(f, Df);

    // a point in the reference configuration
    mito::vector<mito::DIM2> X = {0.0, 0.0};

    // evaluate f and its gradient
    std::cout << "Evaluating cosine function cos(x[0] * x[1]) at X = " << X << " : " << 
        cosine(X) << std::endl; 
    std::cout << "Evaluating gradient of cosine function at X = " << X << " : " << 
        mito::Grad<mito::DIM2>(cosine, X) << std::endl; 

    // instantiate a vector function object
    mito::VectorField<2, mito::DIM2> cosineVector({cosine, cosine});

    // evaluate divergence
    std::cout << "Evaluating divergence of cosine vector function at X = " << X << " : " << 
        mito::Div<mito::DIM2>(cosineVector, X) << std::endl; 

    // 
    mito::VectorField<2, mito::DIM2> gradient = mito::Grad<mito::DIM2>(cosine);
    std::cout << "Evaluating gradient of cosine function at X = " << X << " : " << 
        gradient(X) << std::endl; 

    return 0;
}

// end of file
