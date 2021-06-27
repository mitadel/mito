#include <cmath>
#include "../../math/function.h"
#include "../../math/Field.h"
#include "../../mesh/element_set.h"
#include "../../quadrature/integrator.h"

using mito::Function;
using mito::vector_t;
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
    Function<vector_t<2>, vector_t<2>> f([](const vector_t<2> & x) {
        return vector_t<2> { x[0] * x[1], x[0] * x[0] };
    });

    // df/dx[0]
    Function<vector_t<2>, vector_t<2>> Dx([](const vector_t<2> & x) {
        return vector_t<2> { x[1], 2.0 * x[0] };
    });

    // df/dx[1]
    Function<vector_t<2>, vector_t<2>> Dy([](const vector_t<2> & x) {
        return vector_t<2> { x[0], 0.0 };
    });

    // its partial derivatives
    std::array<Function<vector_t<2>, vector_t<2>>, 2> Df = { Dx, Dy };

    // instantiate a vector field
    mito::VectorField<2 /* D */, 2 /* N */> field(f, Df);

    // build a scalar field with divergence of field
    mito::ScalarField<2> divergence = mito::div(field);

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
    vertexCoordinatesMap.insert(vertex0, point_t<2> { 0.0, 0.0 });
    vertex_t vertex1;
    vertexCoordinatesMap.insert(vertex1, point_t<2> { 1.0, 0.0 });
    vertex_t vertex2;
    vertexCoordinatesMap.insert(vertex2, point_t<2> { 1.0, 1.0 });
    vertex_t vertex3;
    vertexCoordinatesMap.insert(vertex3, point_t<2> { 0.5, 0.5 });
    vertex_t vertex4;
    vertexCoordinatesMap.insert(vertex4, point_t<2> { 0.0, 1.0 });

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
    mito::Integrator<GAUSS, 2 /* degree of exactness */, mito::ElementSet<mito::triangle_t, 2>>
        bodyIntegrator(bodyElementSet);

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
    mito::Integrator<GAUSS, 2 /* degree of exactness */, mito::ElementSet<mito::segment_t, 2>>
        boundaryBotIntegrator(boundaryBot);

    mito::ElementSet boundaryRight(std::vector<segment_t *> { &segment3 }, vertexCoordinatesMap);
    mito::Integrator<GAUSS, 2 /* degree of exactness */, mito::ElementSet<mito::segment_t, 2>>
        boundaryRightIntegrator(boundaryRight);

    mito::ElementSet boundaryTop(std::vector<segment_t *> { &segment6 }, vertexCoordinatesMap);
    mito::Integrator<GAUSS, 2 /* degree of exactness */, mito::ElementSet<mito::segment_t, 2>>
        boundaryTopIntegrator(boundaryTop);

    mito::ElementSet boundaryLeft(std::vector<segment_t *> { &segment7 }, vertexCoordinatesMap);
    mito::Integrator<GAUSS, 2 /* degree of exactness */, mito::ElementSet<mito::segment_t, 2>>
        boundaryLeftIntegrator(boundaryLeft);

    mito::ScalarField<2> f0(f[0]);
    mito::ScalarField<2> f1(f[1]);

    real resultBoundary =
        -boundaryBotIntegrator.integrate(f1) + boundaryRightIntegrator.integrate(f0)
        + boundaryTopIntegrator.integrate(f1) - boundaryLeftIntegrator.integrate(f0);

    std::cout << "Result of boundary integration = " << resultBoundary << std::endl;

    // assert divergence theorem
    assert(std::fabs(resultBody - resultBoundary) < 1.e-15);

    // all done
    return 0;
}

// end of file
