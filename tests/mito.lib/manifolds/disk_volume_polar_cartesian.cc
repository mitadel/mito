#include <gtest/gtest.h>
#include <mito/mito.h>


template <mito::geometry::CoordinateType coordT>
auto
area(std::string mesh_file_name) -> mito::scalar_t
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2, coordT>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // read the mesh
    auto filestream = std::ifstream(mesh_file_name);
    auto mesh =
        mito::io::summit::reader<mito::topology::triangle_t, 2, coordT>(filestream, geometry);

    // create a manifold on {mesh}
    auto manifold = mito::manifolds::manifold<coordT>(mesh);

    // compute the area of the disk
    mito::scalar_t area = manifold.volume();

    // all done
    return area;
}


TEST(Manifolds, Disk)
{
    // compute the area of the disk parametrized in polar coordinates
    mito::scalar_t area_polar = area<mito::geometry::POLAR>("disk_polar.summit");
    // compute the area of the disk parametrized in cartesian coordinates
    mito::scalar_t area_cartesian = area<mito::geometry::EUCLIDEAN>("disk_cartesian.summit");

    // expect to get the same result
    EXPECT_NEAR(area_cartesian, area_polar, 1.e-14);

    // exact area: the area of a disk with unit radius is {pi}
    mito::scalar_t exact = std::numbers::pi;

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the boundary of the disk)
    EXPECT_NEAR(exact, area_cartesian, 0.01);
}

// end of file
