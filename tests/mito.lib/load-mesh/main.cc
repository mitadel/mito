#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

// TOFIX: merge this test with test in directory {mesh}

TEST(LoadMesh, TestLoadMesh)
{
    // an empty topology
    auto topology = mito::topology::topology();

    // an empty cloud of points
    auto point_cloud = mito::geometry::point_cloud<2>();

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::mesh::summit<2>(fileStream, topology, point_cloud);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;

    t = clock();
    auto boundary_elements = mito::mesh::boundary<1>(mesh, topology);
    std::cout << "Fetched boundary in " << clock() - t << std::endl;
}
