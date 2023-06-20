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
     * Mesh with four cells:
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

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    auto vertex0 = geometry.node({ 0.0, 0.0 });
    auto vertex1 = geometry.node({ 1.0, 0.0 });
    auto vertex2 = geometry.node({ 1.0, 1.0 });
    auto vertex3 = geometry.node({ 0.5, 0.5 });
    auto vertex4 = geometry.node({ 0.0, 1.0 });

    auto segment0 = topology.segment({ vertex0, vertex1 });
    auto segment1 = topology.segment({ vertex1, vertex3 });
    auto segment2 = topology.segment({ vertex3, vertex0 });
    auto cell0 = topology.triangle({ segment0, segment1, segment2 });

    auto segment3 = topology.segment({ vertex1, vertex2 });
    auto segment4 = topology.segment({ vertex2, vertex3 });
    auto segment5 = topology.segment({ vertex3, vertex1 });
    auto cell1 = topology.triangle({ segment3, segment4, segment5 });

    auto segment6 = topology.segment({ vertex2, vertex4 });
    auto segment7 = topology.segment({ vertex4, vertex3 });
    auto segment8 = topology.segment({ vertex3, vertex2 });
    auto cell2 = topology.triangle({ segment6, segment7, segment8 });

    auto segment9 = topology.segment({ vertex4, vertex0 });
    auto segment10 = topology.segment({ vertex0, vertex3 });
    auto segment11 = topology.segment({ vertex3, vertex4 });
    auto cell3 = topology.triangle({ segment9, segment10, segment11 });

    mesh.insert(cell0);
    mesh.insert(cell1);
    mesh.insert(cell2);
    mesh.insert(cell3);

    auto bodyManifold = mito::manifolds::manifold(mesh);
    // This instantiates a quad rule on the elements (pairing element type and degree of exactness)
    auto bodyIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyManifold);

    real resultBody = bodyIntegrator.integrate(divergence);
    std::cout << "Result of body integration = " << resultBody << std::endl;

    // TOFIX: Include normal notion on the boundary element set, so that we can avoid hardcoding
    // the normals calculations (we might need std::inner_product to do the inner product)
    /*
    mito::manifolds::Manifold boundaryManifold(
        mito::topology::element_vector_t<segment_t> { &segment0, &segment3, &segment6, &segment7 });
    */
    // integrator on the bottom boundary
    auto meshBot = mito::mesh::mesh<mito::topology::segment_t>(geometry);
    meshBot.insert(segment0);
    auto boundaryBot = mito::manifolds::manifold(meshBot);
    auto boundaryBotIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryBot);

    // integrator on the right boundary
    auto meshRight = mito::mesh::mesh<mito::topology::segment_t>(geometry);
    meshRight.insert(segment3);
    auto boundaryRight = mito::manifolds::manifold(meshRight);
    auto boundaryRightIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryRight);

    // integrator on the top boundary
    auto meshTop = mito::mesh::mesh<mito::topology::segment_t>(geometry);
    meshTop.insert(segment6);
    auto boundaryTop = mito::manifolds::manifold(meshTop);
    auto boundaryTopIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryTop);

    // integrator on the left boundary
    auto meshLeft = mito::mesh::mesh<mito::topology::segment_t>(geometry);
    meshLeft.insert(segment9);
    auto boundaryLeft = mito::manifolds::manifold(meshLeft);
    auto boundaryLeftIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryLeft);

    auto fbot = mito::math::field(-f[1]);
    auto fright = mito::math::field(f[0]);
    auto ftop = mito::math::field(f[1]);
    auto fleft = mito::math::field(-f[0]);

    real resultBoundary =
        boundaryBotIntegrator.integrate(fbot) + boundaryRightIntegrator.integrate(fright)
        + boundaryTopIntegrator.integrate(ftop) + boundaryLeftIntegrator.integrate(fleft);

    std::cout << "Result of boundary integration = " << resultBoundary << std::endl;

    // assert divergence theorem
    EXPECT_DOUBLE_EQ(resultBody, resultBoundary);
}

// end of file
