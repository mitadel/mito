#include <cmath>
#include "../../math/fields.h"
#include "../../mesh/element_set.h"
#include "../../quadrature/integrator.h"

using mito::function;
using mito::vector;
using mito::real;
using mito::DIM1;
using mito::DIM2;
using mito::DIM3;
using mito::GAUSS;
using mito::point_t;
using mito::vertex_t;
using mito::segment_t;
using mito::triangle_t;

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
    mito::Field<DIM2> f_cosine(f, Df);
    mito::ScalarField<DIM2> cosine({ f_cosine });

    // a point in the reference configuration
    vector<DIM2> X = { 0.0, 0.0 };

    // evaluate f and its gradient
    std::cout << "Evaluating cosine function cos(x[0] * x[1]) at X = " << X << " : " << f_cosine(X)
              << std::endl;
    std::cout << "Evaluating gradient of cosine function at X = " << X << " : "
              << mito::grad<DIM2>(cosine, X) << std::endl;

    // instantiate a vector function object
    mito::VectorField<DIM2 /* D */, DIM2 /* N */> cosineVector({ f_cosine, f_cosine });

    // evaluate divergence
    std::cout << "Evaluating divergence of cosine vector function at X = " << X << " : "
              << mito::divX<DIM2>(cosineVector, X) << std::endl;

    // build vector field with gradient of cosine function
    mito::VectorField<DIM2 /* D */, DIM2 /* N */> gradient = mito::grad(cosine);
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

    vertex_t vertex0;
    vertexCoordinatesMap.insert(vertex0, point_t<DIM2>({ 0.0, 0.0 }));
    vertex_t vertex1;
    vertexCoordinatesMap.insert(vertex1, point_t<DIM2>({ 1.0, 0.0 }));
    vertex_t vertex2;
    vertexCoordinatesMap.insert(vertex2, point_t<DIM2>({ 1.0, 1.0 }));
    vertex_t vertex3;
    vertexCoordinatesMap.insert(vertex3, point_t<DIM2>({ 0.5, 0.5 }));
    vertex_t vertex4;
    vertexCoordinatesMap.insert(vertex4, point_t<DIM2>({ 0.0, 1.0 }));

    segment_t segment0({ &vertex0, &vertex1 });
    segment_t segment1({ &vertex1, &vertex3 });
    segment_t segment2({ &vertex3, &vertex0 });
    segment_t segment3({ &vertex1, &vertex2 });
    segment_t segment4({ &vertex2, &vertex3 });
    segment_t segment5({ &vertex4, &vertex3 });
    segment_t segment6({ &vertex2, &vertex4 });
    segment_t segment7({ &vertex4, &vertex0 });

    triangle_t element0({ &segment0, &segment1, &segment2 });
    triangle_t element1({ &segment3, &segment4, &segment1 });
    triangle_t element2({ &segment6, &segment5, &segment4 });
    triangle_t element3({ &segment7, &segment2, &segment5 });

    std::vector<triangle_t *> elements = { &element0, &element1, &element2, &element3 };

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    // static mito::ElementSetTri elementSet;
    mito::ElementSet bodyElementSet(std::move(elements), vertexCoordinatesMap);
    mito::Integrator<GAUSS, triangle_t, 2 /* degree of exactness */, DIM2> bodyIntegrator(
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
    mito::Field<DIM2> f_one([]([[maybe_unused]] const vector<DIM2> & x) { return 1.0; });
    mito::ScalarField<DIM2> one({ f_one });
    result = bodyIntegrator.integrate(one);    // exact 1.0
    std::cout << "Integration of 1: Result = " << result << ", Error = " << std::fabs(result - 1.0)
              << std::endl;
    assert(std::fabs(result - 1.0) < 1.e-16);

    // instantiate a scalar function object
    mito::Field<DIM2> f_linear([](const vector<DIM2> & x) { return x[0]; });
    mito::ScalarField<DIM2> linear({ f_linear });
    result = bodyIntegrator.integrate(linear);    // exact 0.5
    std::cout << "Integration of x: Result = " << result << ", Error = " << std::fabs(result - 0.5)
              << std::endl;
    assert(std::fabs(result - 0.5) < 1.e-16);

    // instantiate a scalar function object
    mito::Field<DIM2> f_xy([](const vector<DIM2> & x) { return x[0] * x[1]; });
    mito::ScalarField<DIM2> xy({ f_xy });
    result = bodyIntegrator.integrate(xy);    // exact 0.25
    std::cout << "Integration of x*y: Result = " << result
              << ", Error = " << std::fabs(result - 0.25) << std::endl;
    assert(std::fabs(result - 0.25) < 1.e-16);

    // instantiate a scalar function object
    mito::Field<DIM2> f_xx([](const vector<DIM2> & x) { return x[0] * x[0]; });
    mito::ScalarField<DIM2> xx({ f_xx });
    result = bodyIntegrator.integrate(xx);    // exact 1.0/3.0
    std::cout << "Integration of x*x: Result = " << result
              << ", Error = " << std::fabs(result - 1.0 / 3.0) << std::endl;
    assert(std::fabs(result - 1.0 / 3.0) < 1.e-16);

    // attach different coordinates (3D coordinates to the same vertices as above)
    mito::VertexCoordinatesMap<DIM3> vertexCoordinatesMap3D;
    vertexCoordinatesMap3D.insert(vertex0, point_t<DIM3>({ 0.0, 0.0, 0.0 }));
    vertexCoordinatesMap3D.insert(vertex1, point_t<DIM3>({ 1.0, 0.0, 1.0 }));
    vertexCoordinatesMap3D.insert(vertex2, point_t<DIM3>({ 1.0, 1.0, 1.0 }));
    vertexCoordinatesMap3D.insert(vertex3, point_t<DIM3>({ 0.5, 0.5, 0.5 }));
    vertexCoordinatesMap3D.insert(vertex4, point_t<DIM3>({ 0.0, 1.0, 0.0 }));

    // instantiate an element set with the same elements as above but the new coordinates map
    std::vector<triangle_t *> elements3D = { &element0, &element1, &element2, &element3 };
    mito::ElementSet bodyElementSet3D(std::move(elements3D), vertexCoordinatesMap3D);

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    // static mito::ElementSetTri elementSet;
    mito::Integrator<GAUSS, triangle_t, 2 /* degree of exactness */, DIM3> bodyIntegrator3D(
        bodyElementSet3D);

    // instantiate a scalar function object
    mito::Field<DIM3> f_xy3D([](const vector<DIM3> & x) { return x[0] * x[1]; });
    mito::ScalarField<DIM3> xy3D({ f_xy3D });
    result = bodyIntegrator3D.integrate(xy3D);    // exact 0.35355339059327384
    std::cout << "Integration of x*y in 3D: Result = " << result
              << ", Error = " << std::fabs(result - 0.35355339059327384) << std::endl;
    assert(std::fabs(result - 0.35355339059327384) < 1.e-15);

    return 0;
}

// end of file
