#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

TEST(EraseElement, DISABLED_TestEraseElement)
{
    // an empty topology
    auto topology = mito::topology::topology();

    // an empty cloud of points
    auto point_cloud = mito::geometry::point_cloud<2>();

    // load mesh of the unit square
    std::ifstream fileStream("small-square.summit");
    auto mesh = mito::mesh::summit<2, mito::topology::simplex_t>(fileStream, topology, point_cloud);

    // show me the points
    // std::cout << "Point cloud: " << std::endl;
    // std::cout << mesh.vertices() << std::endl;

    // assert you read 4 cells
    EXPECT_EQ(mesh.cells<2>().size(), 4);

    // assert the boundary is made of 4 cells
    EXPECT_EQ(mito::mesh::boundary<1>(mesh, topology).size(), 4);

    // show me the cells
    // std::cout << "Initial mesh: " << std::endl;
    // for (const auto & simplex : mesh.cells<2>())
    // {
    //     std::cout << *simplex << std::endl;
    // }

    // erase a simplex
    std::cout << "Erasing simplex..." << std::endl;
    for (const auto & simplex : mesh.cells<2>()) {
        // std::cout << *simplex << std::endl;
        mesh.erase(simplex);
        break;
    }

    // std::cout << "After erase: " << std::endl;
    // for (const auto & simplex : mesh.cells<2>()) {
    //     std::cout << *simplex << std::endl;
    // }

    // assert the boundary is now made of 5 cells
    EXPECT_EQ(mito::mesh::boundary<1>(mesh, topology).size(), 5);

    // show me the boundary cells
    // std::cout << "Boundary: " << std::endl;
    // for (const auto & simplex : mito::mesh::boundary<1>(mesh, topology)) {
    //     std::cout << *simplex << std::endl;
    // }
}
