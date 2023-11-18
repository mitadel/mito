#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(Manifolds, Disk2D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // read the mesh
    auto filestream = std::ifstream("disk_polar.summit");
    auto mesh = mito::io::summit::reader<mito::topology::triangle_t, 2>(filestream, geometry);

    // create a manifold on {mesh} with polar metric
    auto manifold = mito::manifolds::manifold<mito::manifolds::POLAR>(mesh);
    // TOFIX: this method {volume} should be removed as it does not work for nonconstant metrics
    // mito::scalar_t area = manifold.volume();

    auto integrator =
        mito::quadrature::integrator<mito::quadrature::GAUSS, 2 /* degree of exactness */>(
            manifold);
    // a scalar field
    auto one =
        mito::manifolds::field([](const mito::vector_t<2> &) -> mito::scalar_t { return 1.0; });
    // integrate the field
    auto area = integrator.integrate(one);
    std::cout << area << std::endl;
}

// end of file
