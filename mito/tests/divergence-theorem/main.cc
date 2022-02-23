#include "../../base.h"
#include "../../math.h"
#include "../../mesh.h"
#include "../../quadrature.h"

using mito::math::function_t;
using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;
using mito::mesh::point_t;
using mito::mesh::vertex_t;
using mito::mesh::segment_t;
using mito::mesh::triangle_t;

int
main()
{
    // a scalar function
    auto f = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
        return { x[0] * x[1], x[0] * x[0] };
    });

    // df/dx[0]
    auto Dx = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
        return { x[1], 2.0 * x[0] };
    });

    // df/dx[1]
    auto Dy = mito::math::function([](const vector_t<2> & x) -> vector_t<2> {
        return { x[0], 0.0 };
    });

    // its partial derivatives
    auto Df = std::array<function_t<vector_t<2>, vector_t<2>>, 2> { Dx, Dy };

    // instantiate a vector field
    auto field = mito::math::vector_field(f, Df);

    // build a scalar field with divergence of field
    auto divergence = mito::math::div(field);

    // analytic solution
    // int_{int} (div f) = int_0^1 int_0^1 y dx dy = 0.5
    // int_{boundary} (f.n) = int_{right} (f.n) + int_{top} + int_{left} + int_{bot}
    // int_{right} (f.n) = int_{right} (f0) = int_0^1 xy|(x=1) dy = int_0^1 y dy = 0.5
    // int_{top} (f.n) = int_{top} (f1) = int_1^0 x^2 dx = int_0^1 x^2 dx = 1/3
    // int_{left} (f.n) = - int_{left} (f0) = int_0^1 xy|(x=0) dy = 0
    // int_{bot} (f.n) = - int_{bot} (f1) = - int_0^1 x^2 dx = - 1/3

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

    auto vertices = mito::mesh::vertex_set<2>();

    vertex_t vertex0;
    vertices.insert(vertex0, point_t<2> { 0.0, 0.0 });
    vertex_t vertex1;
    vertices.insert(vertex1, point_t<2> { 1.0, 0.0 });
    vertex_t vertex2;
    vertices.insert(vertex2, point_t<2> { 1.0, 1.0 });
    vertex_t vertex3;
    vertices.insert(vertex3, point_t<2> { 0.5, 0.5 });
    vertex_t vertex4;
    vertices.insert(vertex4, point_t<2> { 0.0, 1.0 });

    auto segment0 = mito::mesh::segment({ &vertex0, &vertex1 });
    auto segment1 = mito::mesh::segment({ &vertex1, &vertex3 });
    auto segment2 = mito::mesh::segment({ &vertex3, &vertex0 });
    auto segment3 = mito::mesh::segment({ &vertex1, &vertex2 });
    auto segment4 = mito::mesh::segment({ &vertex2, &vertex3 });
    auto segment5 = mito::mesh::segment({ &vertex4, &vertex3 });
    auto segment6 = mito::mesh::segment({ &vertex2, &vertex4 });
    auto segment7 = mito::mesh::segment({ &vertex4, &vertex0 });

    auto element0 = mito::mesh::triangle({ &segment0, &segment1, &segment2 });
    auto element1 = mito::mesh::triangle({ &segment3, &segment4, &segment1 });
    auto element2 = mito::mesh::triangle({ &segment6, &segment5, &segment4 });
    auto element3 = mito::mesh::triangle({ &segment7, &segment2, &segment5 });

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    // static mito::mesh::ElementSetTri elementSet;
    auto bodyElementSet = mito::mesh::element_set(
        std::vector<triangle_t *> { &element0, &element1, &element2, &element3 }, vertices);
    auto bodyIntegrator = 
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyElementSet);

    real resultBody = bodyIntegrator.integrate(divergence);
    std::cout << "Result of body integration = " << resultBody << std::endl;

    // TOFIX: Include normal notion on the boundary element set, so that we can avoid hardcoding
    // the normals calculations (we might need std::inner_product to do the inner product)
    /*
    mito::mesh::ElementSet boundaryElementSet(
        std::vector<segment_t *> { &segment0, &segment3, &segment6, &segment7 },
        vertices);
    */
    // integrator on the bottom boundary
    auto boundaryBot = mito::mesh::element_set(std::vector<segment_t *> { &segment0 }, vertices);
    auto boundaryBotIntegrator = 
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryBot);

    // integrator on the right boundary
    auto boundaryRight = mito::mesh::element_set(std::vector<segment_t *> { &segment3 }, vertices);
    auto boundaryRightIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryRight);

    // integrator on the top boundary
    auto boundaryTop = mito::mesh::element_set(std::vector<segment_t *> { &segment6 }, vertices);
    auto boundaryTopIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryTop);

    // integrator on the left boundary
    auto boundaryLeft = mito::mesh::element_set(std::vector<segment_t *> { &segment7 }, vertices);
    auto boundaryLeftIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryLeft);

    auto f0 = mito::math::scalar_field(f[0]);
    auto f1 = mito::math::scalar_field(f[1]);

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
