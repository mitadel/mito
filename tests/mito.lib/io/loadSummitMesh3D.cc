#include <gtest/gtest.h>
#include <mito/mito.h>


// TODO: add test on triangle in 3D

TEST(MeshReader, LoadSummitMesh3D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("cube.summit");
    auto mesh = mito::io::summit::reader<mito::topology::simplex_t<3>>(fileStream, geometry);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;
}