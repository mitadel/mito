#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/quadrature.h>


using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;


TEST(Quadrature, Segment)
{
    // a Euclidean coordinate system in 1D
    auto coord_system = mito::geometry::coordinate_system<1, mito::geometry::EUCLIDEAN>();

    // an empty mesh of simplicial topology in 1D
    auto mesh = mito::mesh::mesh<mito::geometry::segment_t<1>>();

    // a mesh of the segment (0, 1)
    auto node_0 = mito::geometry::node(coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0 });
    mesh.insert({ node_0, node_1 });
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, 12);

    // an integrator with degree of exactness 2 on segment (0, 1)
    auto manifold = mito::manifolds::manifold(tetra_mesh, coord_system);
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    // a scalar function
    using coordinates_t = mito::geometry::coordinates_t<1, mito::geometry::EUCLIDEAN>;
    auto f_exp = mito::manifolds::field(
        mito::math::function([](const coordinates_t & x) -> mito::scalar_t { return exp(-x[0]); }));

    // integrate exp(-x) on (0, 1)
    auto integral = integrator.integrate(f_exp);
    EXPECT_NEAR(integral, (exp(1) - 1) / exp(1), 1.e-13);
}

// end of file