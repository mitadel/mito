#include <gtest/gtest.h>
#include <mito/mito.h>


using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;
using mito::topology::triangle_t;


TEST(Quadrature, Segment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<1>();

    // a 1D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 1D
    auto mesh = mito::mesh::mesh<mito::topology::segment_t>(geometry);

    // a mesh of the segment (0, 1)
    auto vertex0 = geometry.node({ 0.0 });
    auto vertex1 = geometry.node({ 1.0 });
    auto segment0 = topology.segment({ vertex0, vertex1 });
    mesh.insert(segment0);
    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, 12);

    // an integrator with degree of exactness 2 on segment (0, 1)
    auto manifold = mito::manifolds::manifold(tetra_mesh);
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    // a scalar function
    auto f_exp = mito::manifolds::field(mito::math::function(
        [](const mito::vector_t<1> & x) -> mito::scalar_t { return exp(-x[0]); }));

    // integrate exp(-x) on (0, 1)
    auto integral = integrator.integrate(f_exp);
    EXPECT_NEAR(integral, (exp(1) - 1) / exp(1), 1.e-13);
}

// end of file
