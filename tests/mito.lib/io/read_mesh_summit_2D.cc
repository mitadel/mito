#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(SummitReader, LoadMesh2D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("rectangle.summit");
    auto mesh =
        mito::io::summit::reader<mito::topology::simplex_t<2>>(fileStream, geometry, coord_system);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;

    t = clock();
    auto boundary_mesh = mesh.boundary();
    std::cout << "Fetched boundary in " << clock() - t << std::endl;
}
