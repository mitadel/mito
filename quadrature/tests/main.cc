#include "../../mito.h"
#include "../quadrature.h"

using mito::DIM2;
using mito::TRI;

int main () {

    // vertices of one element
    std::vector<mito::vector<DIM2> > verticesElement(3);
    verticesElement[0][0] = 0.0; 
    verticesElement[0][1] = 0.0; 
    verticesElement[1][0] = 1.0; 
    verticesElement[1][1] = 0.0; 
    verticesElement[2][0] = 0.5; 
    verticesElement[2][1] = 0.5; 

    std::cout << "One-point quadrature rule (r = 1) : " << std::endl;
    const auto quadratureRule1 = mito::QuadratureRule<TRI, 1>();
    std::cout << "Quad Points (reference): " << quadratureRule1.points() << std::endl;
    std::cout << "Quad Weights: " << quadratureRule1.weights() << std::endl;

    const std::vector<mito::vector<DIM2> > quadCoordinates1 = 
        quadratureRule1.quadraturePointsCurrentElement(verticesElement);
    std::cout << "Quad Points (element) " << quadCoordinates1 << std::endl;

    std::cout << "Three-point quadrature rule (r = 2):" << std::endl;
    const auto quadratureRule2 = mito::QuadratureRule<TRI, 2>();
    std::cout << "Quad Points (reference): " << quadratureRule2.points() << std::endl;
    std::cout << "Quad Weights: " << quadratureRule2.weights() << std::endl;

    const std::vector<mito::vector<DIM2> > quadCoordinates2 = 
        quadratureRule2.quadraturePointsCurrentElement(verticesElement);
    std::cout << "Quad Points (element) " << quadCoordinates2 << std::endl;

    return 0;
}

// end of file
