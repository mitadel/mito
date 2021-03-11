#include <cmath>
#include "../math.h"
#include "../point.h"
#include "../../elements/elements.h"
#include "../../quadrature/quadrature.h"

using mito::function;
using mito::vector;
using mito::real;
using mito::DIM1;
using mito::DIM2;
using mito::DIM3;
using mito::point;
using mito::x0;
using mito::x1;
using mito::x2;
using mito::x3;

// TODO: When everything is in place to compute integrals, add Stokes' theorem as a test.

int main () {

    point<DIM1> a(10.0);
    std::cout << a << std::endl;

    point<DIM2> b(10.0, 2.0);
    std::cout << b << std::endl;

    point<DIM3> c(10.0, 2.0, 1.0);
    std::cout << c << std::endl;

    // a scalar function 
    function<vector<DIM2>> f{ 
        [](const vector<DIM2>& x){ return cos(x[0] * x[1]); } };

    // df/dx[0]
    function<vector<DIM2>> Dx{ 
        [](const vector<DIM2>& x){ return -sin(x[0] * x[1])* x[1]; } };

    // df/dx[1] 
    function<vector<DIM2>> Dy{ 
        [](const vector<DIM2>& x){ return -sin(x[0] * x[1])* x[0]; } };

    // its partial derivatives 
    std::array<function<vector<DIM2>>, DIM2> Df = {Dx, Dy};

    // instantiate a scalar function object
    mito::ScalarField<DIM2> cosine(f, Df);

    // a point in the reference configuration
    vector<DIM2> X = {0.0, 0.0};

    // evaluate f and its gradient
    std::cout << "Evaluating cosine function cos(x[0] * x[1]) at X = " << X << " : " << 
        cosine(X) << std::endl; 
    std::cout << "Evaluating gradient of cosine function at X = " << X << " : " << 
        mito::gradX<DIM2>(cosine, X) << std::endl; 

    // instantiate a vector function object
    mito::VectorField<DIM2 /* N */, DIM2 /* D */> cosineVector({cosine, cosine});

    // evaluate divergence
    std::cout << "Evaluating divergence of cosine vector function at X = " << X << " : " << 
        mito::divX<DIM2>(cosineVector, X) << std::endl; 

    // build vector field with gradient of cosine function 
    mito::VectorField<DIM2 /* N */, DIM2 /* D */> gradient = mito::gradX(cosine);
    std::cout << "Evaluating gradient of cosine function at X = " << X << " : " << 
        gradient(X) << std::endl; 

    /**
     * Mesh with four elements:
        (0,1)           (1,1)
          4               2
          +---------------+
          | .           . |
          |   .       .   |
          |     .   .     |
          |       . 3     |
          |     .   .     |
          |   .       .   |
          | .           . |
          +---------------+
          0               1
        (0,0)           (1,0)
    */

    // connectivity of the mesh
    mito::Connectivity<3 /* nodes per element */> connectivity (4 /* number of elements */, 
        { 0, 1, 3, // Element 0: Nodes 0, 1, 3
          1, 2, 3, // Element 1: Nodes 1, 2, 3
          3, 2, 4, // Element 2: Nodes 3, 2, 4
          0, 3, 4  // Element 3: Nodes 0, 3, 4
        });
    std::cout << "Connectivity: " << connectivity() << std::endl;

    // coordinates of the mesh nodes
    mito::NodalField<real> coordinates(5, 2, "coordinates");
    coordinates(0, 0) = 0.0;
    coordinates(0, 1) = 0.0;              /* Node 0 */
    coordinates(1, 0) = 1.0;
    coordinates(1, 1) = 0.0;              /* Node 1 */
    coordinates(2, 0) = 1.0;
    coordinates(2, 1) = 1.0;              /* Node 2 */
    coordinates(3, 0) = 0.5;
    coordinates(3, 1) = 0.5;              /* Node 3 */
    coordinates(4, 0) = 0.0;
    coordinates(4, 1) = 1.0;              /* Node 4 */
    std::cout << coordinates << std::endl;

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    //static mito::ElementSetTri elementSet;
    mito::ElementSetTri<1 /* polynomial order */> elementSet(connectivity, coordinates);
    mito::Integrator<mito::GAUSS, mito::TRI, 2 /* degree of exactness */> 
        integrator(elementSet.elements());
#if 1
    static const auto _quadratureRule = 
        mito::SampleQuadratureRule<mito::GAUSS, mito::TRI, 2>::Get();
    std::cout << "\t " << std::get<0>(_quadratureRule[0])[0] << std::endl;
#else
    real result = integrator.integrate(cosine);     // exact 0.946083...
    result *= 0.5; /* 0.5 is to adjust the missing jacobian multiplication */
    std::cout << "Integration of cos(x*y): Result = " << result
        << ", Error = " << std::fabs(result - 0.946083)
        << std::endl;

    // instantiate a scalar function object
    mito::ScalarField<DIM2> one([](const vector<DIM2>& x){ return 1.0; });
    result = integrator.integrate(one);     // exact 1.0
    result *= 0.5; /* 0.5 is to adjust the missing jacobian multiplication */
    std::cout << "Integration of 1: Result = " << result 
        << ", Error = " << std::fabs(result - 1.0)
        << std::endl;

    // instantiate a scalar function object
    mito::ScalarField<DIM2> linear([](const vector<DIM2>& x){ return x[0]; });
    result = integrator.integrate(linear);  // exact 0.5
    result *= 0.5; /* 0.5 is to adjust the missing jacobian multiplication */
    std::cout << "Integration of x: Result = " << result 
        << ", Error = " << std::fabs(result - 0.5)
        << std::endl;

    // instantiate a scalar function object
    mito::ScalarField<DIM2> xy([](const vector<DIM2>& x){ return x[0]*x[1]; });
    result = integrator.integrate(xy);      // exact 0.25
    result *= 0.5; /* 0.5 is to adjust the missing jacobian multiplication */
    std::cout << "Integration of x*y: Result = " << result 
        << ", Error = " << std::fabs(result - 0.25)
        << std::endl;

    // instantiate a scalar function object
    mito::ScalarField<DIM2> xx([](const vector<DIM2>& x){ return x[0]*x[0]; });
    result = integrator.integrate(xx);      // exact 1.0/3.0
    result *= 0.5; /* 0.5 is to adjust the missing jacobian multiplication */
    std::cout << "Integration of x*x: Result = " << result 
        << ", Error = " << std::fabs(result - 1.0/3.0)
        << std::endl;
#endif
    return 0;
}

// end of file
