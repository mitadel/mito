#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

TEST(EraseElement, TestEraseElement)
{
    // load mesh of the unit square
    std::ifstream fileStream("small-square.summit");
    auto mesh = mito::mesh::summit<2>(fileStream);

    // show me the points
    // std::cout << "Point cloud: " << std::endl;
    // std::cout << mesh.vertices() << std::endl;

    // assert you read 4 elements
    EXPECT_EQ(mesh.elements<2>().size(), 4);

    // assert the boundary is made of 4 elements
    EXPECT_EQ(mesh.boundary_elements<1>().size(), 4);

    // show me the elements
    // std::cout << "Initial mesh: " << std::endl;
    // for (const auto & simplex : mesh.elements<2>())
    // {
    //     std::cout << *simplex << std::endl;
    // }

    // erase a simplex
    std::cout << "Erasing simplex..." << std::endl;
    for (const auto & simplex : mesh.elements<2>()) {
        // std::cout << *simplex << std::endl;
        mesh.erase(simplex);
        break;
    }

    // std::cout << "After erase: " << std::endl;
    // for (const auto & simplex : mesh.elements<2>()) {
    //     std::cout << *simplex << std::endl;
    // }

    // assert the boundary is now made of 5 elements
    EXPECT_EQ(mesh.boundary_elements<1>().size(), 5);

    // show me the boundary elements
    // std::cout << "Boundary: " << std::endl;
    // for (const auto & simplex : mesh.boundary_elements<1>()) {
    //     std::cout << *simplex << std::endl;
    // }
}
