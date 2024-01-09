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
    auto manifold = mito::manifolds::manifold<mito::geometry::POLAR>(mesh);
    mito::scalar_t area = manifold.volume();

    // check area calculation
    EXPECT_DOUBLE_EQ(std::numbers::pi / 4.0, area);
}

// end of file
