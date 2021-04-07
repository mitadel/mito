#include "../../mito.h"
#include "../quadrature.h"

using mito::DIM2;
using mito::TRI;
using mito::GAUSS;

int
main()
{

    // vertices of one element
    std::vector<mito::vector<DIM2>> verticesElement(3);
    verticesElement[0][0] = 0.0;
    verticesElement[0][1] = 0.0;
    verticesElement[1][0] = 1.0;
    verticesElement[1][1] = 0.0;
    verticesElement[2][0] = 0.5;
    verticesElement[2][1] = 0.5;

    std::cout << "One-point quadrature rule (r = 1) : " << std::endl;
    const auto quadratureRule1 = mito::SampleQuadratureRule<GAUSS, TRI, 1>::Get();
    // TODO: Overload operator << for quadrature rules
    // These lines do not build with the current implementation of quadrature rules
    // std::cout << "Quad Points (reference): " << quadratureRule1.points() << std::endl;
    // std::cout << "Quad Weights: " << quadratureRule1.weights() << std::endl;

    std::cout << "Three-point quadrature rule (r = 2):" << std::endl;
    const auto quadratureRule2 = mito::SampleQuadratureRule<GAUSS, TRI, 2>::Get();
    // These lines do not build with the current implementation of quadrature rules
    // std::cout << "Quad Points (reference): " << quadratureRule2.points() << std::endl;
    // std::cout << "Quad Weights: " << quadratureRule2.weights() << std::endl;

    return 0;
}

// end of file
