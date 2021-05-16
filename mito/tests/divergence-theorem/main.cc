#include <cmath>
#include "../../math/fields.h"
#include "../../mesh/element_set.h"
#include "../../quadrature/integrator.h"

using mito::function;
using mito::vector;
using mito::real;
using mito::GAUSS;
using mito::point_t;
using mito::vertex_t;
using mito::segment_t;
using mito::triangle_t;

int
main()
{
    // a scalar function
    function<vector<2>> f1 { [](const vector<2> & x) {
        return x[0] * x[1];
    } };

    // df/dx[0]
    function<vector<2>> Dx_f1 { [](const vector<2> & x) {
        return x[1];
    } };

    // df/dx[1]
    function<vector<2>> Dy_f1 { [](const vector<2> & x) {
        return x[0];
    } };

    // its partial derivatives
    std::array<function<vector<2>>, 2> Df1 = { Dx_f1, Dy_f1 };

    // instantiate a scalar function object
    mito::Field<2> field1(f1, Df1);

    // a scalar function
    function<vector<2>> f2 { [](const vector<2> & x) {
        return x[0] * x[0];
    } };

    // df/dx[0]
    function<vector<2>> Dx_f2 { [](const vector<2> & x) {
        return 2.0 * x[0];
    } };

    // df/dx[1]
    function<vector<2>> Dy_f2 { [](const vector<2> & x) {
        return 0.0;
    } };

    // its partial derivatives
    std::array<function<vector<2>>, 2> Df2 = { Dx_f2, Dy_f2 };

    // instantiate a scalar function object
    mito::Field<2> field2(f2, Df2);

    // instantiate a vector function object
    mito::VectorField<2 /* D */, 2 /* N */> vector_field({ field1, field2 });

    // build a scalar field with divergence of cosineVector
    mito::ScalarField<2> divergence = mito::div(vector_field);

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

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    // static mito::ElementSetTri elementSet;
    mito::ElementSet bodyElementSet(
        std::vector<triangle_t *> { &element0, &element1, &element2, &element3 },
        vertexCoordinatesMap);
    mito::Integrator<GAUSS, triangle_t, 2 /* degree of exactness */, 2> bodyIntegrator(
        bodyElementSet);

    real resultBody = bodyIntegrator.integrate(divergence);
    std::cout << "Result of body integration = " << resultBody << std::endl;

    // TOFIX: Include normal notion on the boundary element set, so that we can avoid hardcoding
    // the normals calculations (we might need std::inner_product to do the inner product)
    /*
    mito::ElementSet boundaryElementSet(
        std::vector<segment_t *> { &segment0, &segment3, &segment6, &segment7 },
        vertexCoordinatesMap);
    */
    mito::ElementSet boundaryBot(std::vector<segment_t *> { &segment0 }, vertexCoordinatesMap);
    mito::Integrator<GAUSS, segment_t, 2 /* degree of exactness */, 2> boundaryBotIntegrator(
        boundaryBot);

    mito::ElementSet boundaryRight(std::vector<segment_t *> { &segment3 }, vertexCoordinatesMap);
    mito::Integrator<GAUSS, segment_t, 2 /* degree of exactness */, 2> boundaryRightIntegrator(
        boundaryRight);

    mito::ElementSet boundaryTop(std::vector<segment_t *> { &segment6 }, vertexCoordinatesMap);
    mito::Integrator<GAUSS, segment_t, 2 /* degree of exactness */, 2> boundaryTopIntegrator(
        boundaryTop);

    mito::ElementSet boundaryLeft(std::vector<segment_t *> { &segment7 }, vertexCoordinatesMap);
    mito::Integrator<GAUSS, segment_t, 2 /* degree of exactness */, 2> boundaryLeftIntegrator(
        boundaryLeft);

    real resultBoundary =
        -boundaryBotIntegrator.integrate(mito::ScalarField<2>({ vector_field[1] }))
        + boundaryRightIntegrator.integrate(mito::ScalarField<2>({ vector_field[0] }))
        + boundaryTopIntegrator.integrate(mito::ScalarField<2>({ vector_field[1] }))
        - boundaryLeftIntegrator.integrate(mito::ScalarField<2>({ vector_field[0] }));

    std::cout << "Result of boundary integration = " << resultBoundary << std::endl;

    // assert divergence theorem
    assert(std::fabs(resultBody - resultBoundary) < 1.e-15);

    // all done
    return 0;
}

// end of file
