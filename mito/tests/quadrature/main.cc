#include <cmath>
#include "../../math/fields.h"
#include "../../math/function.h"
#include "../../mesh/element_set.h"
#include "../../quadrature/integrator.h"

using mito::Function;
using mito::vector;
using mito::real;
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
    Function<vector<2>> f([](const vector<2> & x) { return cos(x[0] * x[1]); });

    // df/dx[0]
    Function<vector<2>> Dfx([](const vector<2> & x) { return -sin(x[0] * x[1]) * x[1]; });

    // df/dx[1]
    Function<vector<2>> Dfy([](const vector<2> & x) { return -sin(x[0] * x[1]) * x[0]; });

    // its partial derivatives
    std::array<Function<vector<2>>, 2> Df = { Dfx, Dfy };

    // instantiate a scalar field
    mito::ScalarField<2> f_cosine(f, Df);

    mito::ScalarField<2> cosine_sum1(f + f);
    mito::ScalarField<2> cosine_sum2(f_cosine + f_cosine);

    // a vector function
    Function<vector<2>, vector<2>> g([](const mito::vector<2> & x) {
        return vector<2> { cos(x[0] * x[1]), cos(x[0] * x[1]) };
    });

    // df/dx[0]
    Function<vector<2>, vector<2>> Dgx([](const vector<2> & x) {
        return vector<2> { -sin(x[0] * x[1]) * x[1], -sin(x[0] * x[1]) * x[1] };
    });

    // df/dx[1]
    Function<vector<2>, vector<2>> Dgy([](const vector<2> & x) {
        return vector<2> { -sin(x[0] * x[1]) * x[0], -sin(x[0] * x[1]) * x[0] };
    });

    // its partial derivatives
    std::array<Function<vector<2>, vector<2>>, 2> Dg = { Dgx, Dgy };

    // instantiate a vector field
    mito::VectorField<2 /* D */, 2 /* N */> cosineVector(g, Dg);

    // a point in the reference configuration
    vector<2> x = { 0.0, 0.0 };

    // evaluate f and its gradient
    std::cout << "Evaluating cosine function cos(x[0] * x[1]) at X = " << x << " : " << f_cosine(x)
              << std::endl;
    std::cout << "Evaluating gradient of cosine function at X = " << x << " : "
              << mito::grad(f_cosine, x) << std::endl;
    std::cout << "Evaluating divergence of cosine vector function at x = " << x << " : "
              << mito::div(cosineVector, x) << std::endl;

    auto gradient = mito::grad(f_cosine);
    // evaluate divergence
    auto divergence = mito::div(cosineVector);

    std::cout << "Evaluating gradient of cosine function at x = " << x << " : " << gradient(x)
              << std::endl;
    std::cout << "Evaluating divergence of cosine vector function at X = " << x << " : "
              << divergence(x) << std::endl;

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

    mito::VertexCoordinatesMap<2> vertexCoordinatesMap;

    vertex_t vertex0;
    vertexCoordinatesMap.insert(vertex0, point_t<2>({ 0.0, 0.0 }));
    vertex_t vertex1;
    vertexCoordinatesMap.insert(vertex1, point_t<2>({ 1.0, 0.0 }));
    vertex_t vertex2;
    vertexCoordinatesMap.insert(vertex2, point_t<2>({ 1.0, 1.0 }));
    vertex_t vertex3;
    vertexCoordinatesMap.insert(vertex3, point_t<2>({ 0.5, 0.5 }));
    vertex_t vertex4;
    vertexCoordinatesMap.insert(vertex4, point_t<2>({ 0.0, 1.0 }));

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
    mito::ElementSet bodyElementSet(elements, vertexCoordinatesMap);
    // QUESTION: Is it possible to remove the last 2 in the template parameters?
    mito::Integrator<GAUSS, triangle_t, 2 /* degree of exactness */, 2> bodyIntegrator(
        bodyElementSet);

#if 0
    mito::Elements<SEG, 2> boundaryElements(connectivityBoundary, coordinates);
    mito::Integrator<GAUSS, SEG, 2 /* degree of exactness */, 2> boundaryIntegrator(
        boundaryElements);
#endif

    real result = bodyIntegrator.integrate(f_cosine);    // exact 0.946083...
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    assert(std::fabs(result - 0.946083) < 1.e-3);

    auto resultVector = bodyIntegrator.integrate(cosineVector);
    assert(resultVector == mito::vector<2>({ result, result }));

    // instantiate a scalar function object
    mito::ScalarField<2> one([]([[maybe_unused]] const vector<2> & x) { return 1.0; });
    result = bodyIntegrator.integrate(one);    // exact 1.0
    std::cout << "Integration of 1: Result = " << result << ", Error = " << std::fabs(result - 1.0)
              << std::endl;
    assert(std::fabs(result - 1.0) < 1.e-16);

    // instantiate a scalar function object
    mito::ScalarField<2> linear([](const vector<2> & x) { return x[0]; });
    result = bodyIntegrator.integrate(linear);    // exact 0.5
    std::cout << "Integration of x: Result = " << result << ", Error = " << std::fabs(result - 0.5)
              << std::endl;
    assert(std::fabs(result - 0.5) < 1.e-16);

    // instantiate a scalar function object
    mito::ScalarField<2> xy([](const vector<2> & x) { return x[0] * x[1]; });
    result = bodyIntegrator.integrate(xy);    // exact 0.25
    std::cout << "Integration of x*y: Result = " << result
              << ", Error = " << std::fabs(result - 0.25) << std::endl;
    assert(std::fabs(result - 0.25) < 1.e-16);

    // instantiate a scalar function object
    mito::ScalarField<2> xx([](const vector<2> & x) { return x[0] * x[0]; });
    result = bodyIntegrator.integrate(xx);    // exact 1.0/3.0
    std::cout << "Integration of x*x: Result = " << result
              << ", Error = " << std::fabs(result - 1.0 / 3.0) << std::endl;
    assert(std::fabs(result - 1.0 / 3.0) < 1.e-16);

    // attach different coordinates (3D coordinates to the same vertices as above)
    mito::VertexCoordinatesMap<3> vertexCoordinatesMap3D;
    vertexCoordinatesMap3D.insert(vertex0, point_t<3>({ 0.0, 0.0, 0.0 }));
    vertexCoordinatesMap3D.insert(vertex1, point_t<3>({ 1.0, 0.0, 1.0 }));
    vertexCoordinatesMap3D.insert(vertex2, point_t<3>({ 1.0, 1.0, 1.0 }));
    vertexCoordinatesMap3D.insert(vertex3, point_t<3>({ 0.5, 0.5, 0.5 }));
    vertexCoordinatesMap3D.insert(vertex4, point_t<3>({ 0.0, 1.0, 0.0 }));

    // instantiate an element set with the same elements as above but the new coordinates map
    mito::ElementSet bodyElementSet3D(elements, vertexCoordinatesMap3D);

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    // static mito::ElementSetTri elementSet;
    mito::Integrator<GAUSS, triangle_t, 2 /* degree of exactness */, 3> bodyIntegrator3D(
        bodyElementSet3D);

    // instantiate a scalar function object
    mito::ScalarField<3> xy3D([](const vector<3> & x) { return x[0] * x[1]; });
    result = bodyIntegrator3D.integrate(xy3D);    // exact 0.35355339059327384
    std::cout << "Integration of x*y in 3D: Result = " << result
              << ", Error = " << std::fabs(result - 0.35355339059327384) << std::endl;
    assert(std::fabs(result - 0.35355339059327384) < 1.e-15);

    return 0;
}

// end of file
