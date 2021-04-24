#include <cmath>
#include "../../math/fields.h"
#include "../../elements/elements.h"
#include "../../quadrature/quadrature.h"
#include "../../quadrature/integrator.h"

using mito::function;
using mito::vector;
using mito::real;
using mito::DIM1;
using mito::DIM2;
using mito::DIM3;
using mito::GAUSS;

// TODO: When everything is in place to compute integrals, add Stokes' theorem as a test.

int
main()
{

    // a scalar function
    function<vector<DIM2>> f { [](const vector<DIM2> & x) {
        return cos(x[0] * x[1]);
    } };

    // df/dx[0]
    function<vector<DIM2>> Dx { [](const vector<DIM2> & x) {
        return -sin(x[0] * x[1]) * x[1];
    } };

    // df/dx[1]
    function<vector<DIM2>> Dy { [](const vector<DIM2> & x) {
        return -sin(x[0] * x[1]) * x[0];
    } };

    // its partial derivatives
    std::array<function<vector<DIM2>>, DIM2> Df = { Dx, Dy };

    // instantiate a scalar function object
    mito::ScalarField<DIM2> cosine(f, Df);

    // a point in the reference configuration
    vector<DIM2> X = { 0.0, 0.0 };

    // evaluate f and its gradient
    std::cout << "Evaluating cosine function cos(x[0] * x[1]) at X = " << X << " : " << cosine(X)
              << std::endl;
    std::cout << "Evaluating gradient of cosine function at X = " << X << " : "
              << mito::gradX<DIM2>(cosine, X) << std::endl;

    // instantiate a vector function object
    mito::VectorField<DIM2 /* N */, DIM2 /* D */> cosineVector({ cosine, cosine });

    // evaluate divergence
    std::cout << "Evaluating divergence of cosine vector function at X = " << X << " : "
              << mito::divX<DIM2>(cosineVector, X) << std::endl;

    // build vector field with gradient of cosine function
    mito::VectorField<DIM2 /* N */, DIM2 /* D */> gradient = mito::gradX(cosine);
    std::cout << "Evaluating gradient of cosine function at X = " << X << " : " << gradient(X)
              << std::endl;

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

    mito::VertexCoordinatesMap<DIM2> vertexCoordinatesMap;

    mito::vertex_t vertex0;
    vertexCoordinatesMap.insert(vertex0, mito::point_t<DIM2>({ 0.0, 0.0 }));
    mito::vertex_t vertex1;
    vertexCoordinatesMap.insert(vertex1, mito::point_t<DIM2>({ 1.0, 0.0 }));
    mito::vertex_t vertex2;
    vertexCoordinatesMap.insert(vertex2, mito::point_t<DIM2>({ 1.0, 1.0 }));
    mito::vertex_t vertex3;
    vertexCoordinatesMap.insert(vertex3, mito::point_t<DIM2>({ 0.5, 0.5 }));
    mito::vertex_t vertex4;
    vertexCoordinatesMap.insert(vertex4, mito::point_t<DIM2>({ 0.0, 1.0 }));

    mito::segment_t segment0({ &vertex0, &vertex1 });
    mito::segment_t segment1({ &vertex1, &vertex3 });
    mito::segment_t segment2({ &vertex3, &vertex0 });
    mito::segment_t segment3({ &vertex1, &vertex2 });
    mito::segment_t segment4({ &vertex2, &vertex3 });
    mito::segment_t segment5({ &vertex4, &vertex3 });
    mito::segment_t segment6({ &vertex2, &vertex4 });
    mito::segment_t segment7({ &vertex4, &vertex0 });

    mito::triangle_t element0({ &segment0, &segment1, &segment2 });
    mito::triangle_t element1({ &segment3, &segment4, &segment1 });
    mito::triangle_t element2({ &segment6, &segment5, &segment4 });
    mito::triangle_t element3({ &segment7, &segment2, &segment5 });

    std::vector<mito::triangle_t *> elements = { &element0, &element1, &element2, &element3 };

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    // static mito::ElementSetTri elementSet;
    mito::ElementSet bodyElementSet(std::move(elements), vertexCoordinatesMap);
    mito::Integrator<GAUSS, mito::triangle_t, 2 /* degree of exactness */, DIM2> bodyIntegrator(
        bodyElementSet);

#if 0
    mito::Elements<SEG, DIM2> boundaryElements(connectivityBoundary, coordinates);
    mito::Integrator<GAUSS, SEG, 2 /* degree of exactness */, DIM2> boundaryIntegrator(
        boundaryElements);
#endif

    real result = bodyIntegrator.integrate(cosine);    // exact 0.946083...
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    assert(std::fabs(result - 0.946083) < 1.e-3);

    // instantiate a scalar function object
    mito::ScalarField<DIM2> one([](const vector<DIM2> & x) { return 1.0; });
    result = bodyIntegrator.integrate(one);    // exact 1.0
    std::cout << "Integration of 1: Result = " << result << ", Error = " << std::fabs(result - 1.0)
              << std::endl;
    assert(std::fabs(result - 1.0) < 1.e-16);

    // instantiate a scalar function object
    mito::ScalarField<DIM2> linear([](const vector<DIM2> & x) { return x[0]; });
    result = bodyIntegrator.integrate(linear);    // exact 0.5
    std::cout << "Integration of x: Result = " << result << ", Error = " << std::fabs(result - 0.5)
              << std::endl;
    assert(std::fabs(result - 0.5) < 1.e-16);

    // instantiate a scalar function object
    mito::ScalarField<DIM2> xy([](const vector<DIM2> & x) { return x[0] * x[1]; });
    result = bodyIntegrator.integrate(xy);    // exact 0.25
    std::cout << "Integration of x*y: Result = " << result
              << ", Error = " << std::fabs(result - 0.25) << std::endl;
    assert(std::fabs(result - 0.25) < 1.e-16);

    // instantiate a scalar function object
    mito::ScalarField<DIM2> xx([](const vector<DIM2> & x) { return x[0] * x[0]; });
    result = bodyIntegrator.integrate(xx);    // exact 1.0/3.0
    std::cout << "Integration of x*x: Result = " << result
              << ", Error = " << std::fabs(result - 1.0 / 3.0) << std::endl;
    assert(std::fabs(result - 1.0 / 3.0) < 1.e-16);

    return 0;
}

// end of file
