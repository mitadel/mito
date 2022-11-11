#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/math.h>
#include <mito/mesh.h>
#include <mito/manifolds.h>
#include <mito/quadrature.h>

using mito::math::function_t;
using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;
using mito::topology::segment_t;
using mito::topology::triangle_t;

TEST(DivergenceTheorem, TestDivergenceTheorem)
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

    // instantiate a vector field
    auto field = mito::math::field(f, { Dx, Dy });

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

    // an empty topology
    auto topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto point_cloud = mito::geometry::point_cloud<2>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<2, mito::topology::simplex_t>();

    auto vertex0 = topology.vertex();
    auto point0 = point_cloud.point({ 0.0, 0.0 });
    mesh.addVertex(vertex0, point0);
    auto vertex1 = topology.vertex();
    auto point1 = point_cloud.point({ 1.0, 0.0 });
    mesh.addVertex(vertex1, point1);
    auto vertex2 = topology.vertex();
    auto point2 = point_cloud.point({ 1.0, 1.0 });
    mesh.addVertex(vertex2, point2);
    auto vertex3 = topology.vertex();
    auto point3 = point_cloud.point({ 0.5, 0.5 });
    mesh.addVertex(vertex3, point3);
    auto vertex4 = topology.vertex();
    auto point4 = point_cloud.point({ 0.0, 1.0 });
    mesh.addVertex(vertex4, point4);

    auto segment0 = topology.segment({ vertex0, vertex1 });
    auto segment1 = topology.segment({ vertex1, vertex3 });
    auto segment2 = topology.segment({ vertex3, vertex0 });
    auto segment3 = topology.segment({ vertex1, vertex2 });
    auto segment4 = topology.segment({ vertex2, vertex3 });
    auto segment5 = topology.segment({ vertex4, vertex3 });
    auto segment6 = topology.segment({ vertex2, vertex4 });
    auto segment7 = topology.segment({ vertex4, vertex0 });

    auto element0 = topology.triangle({ segment0, segment1, segment2 });
    auto element1 = topology.triangle({ segment3, segment4, segment1 });
    auto element2 = topology.triangle({ segment6, segment5, segment4 });
    auto element3 = topology.triangle({ segment7, segment2, segment5 });

    mesh.addSimplex(element0);
    mesh.addSimplex(element1);
    mesh.addSimplex(element2);
    mesh.addSimplex(element3);

    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    // static mito::manifolds::ManifoldTri elementSet;
    auto bodyManifold = mito::manifolds::manifold<2>(mesh);
    auto bodyIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyManifold);

    real resultBody = bodyIntegrator.integrate(divergence);
    std::cout << "Result of body integration = " << resultBody << std::endl;

    // TOFIX: fix this part once design of {Manifold} has improved
#if 0
    // TOFIX: Include normal notion on the boundary element set, so that we can avoid hardcoding
    // the normals calculations (we might need std::inner_product to do the inner product)
    /*
    mito::manifolds::Manifold boundaryManifold(
        mito::topology::element_vector_t<segment_t> { &segment0, &segment3, &segment6, &segment7 });
    */
    // integrator on the bottom boundary
    auto boundaryBot =
        mito::manifolds::manifold<2>(mito::topology::element_vector_t<segment_t> { segment0 });
    auto boundaryBotIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryBot);

    // integrator on the right boundary
    auto boundaryRight =
        mito::manifolds::manifold<2>(mito::topology::element_vector_t<segment_t> { segment3 });
    auto boundaryRightIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryRight);

    // integrator on the top boundary
    auto boundaryTop =
        mito::manifolds::manifold<2>(mito::topology::element_vector_t<segment_t> { segment6 });
    auto boundaryTopIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryTop);

    // integrator on the left boundary
    auto boundaryLeft =
        mito::manifolds::manifold<2>(mito::topology::element_vector_t<segment_t> { segment7 });
    auto boundaryLeftIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryLeft);

    auto f0 = mito::math::field(f[0]);
    auto f1 = mito::math::field(f[1]);

    real resultBoundary =
        -boundaryBotIntegrator.integrate(f1) + boundaryRightIntegrator.integrate(f0)
        + boundaryTopIntegrator.integrate(f1) - boundaryLeftIntegrator.integrate(f0);

    std::cout << "Result of boundary integration = " << resultBoundary << std::endl;

    // assert divergence theorem
    EXPECT_NEAR(resultBody, resultBoundary, 1.e-15);
#endif
}

// end of file
