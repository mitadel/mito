#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>


TEST(Mesh, EraseElement)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // instantiate four nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 1.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.5, 0.5 });
    auto node_4 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // an empty mesh of triangles in 2D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<2>>();
    auto geom_cell0 = mesh.insert({ node_0, node_1, node_3 });
    auto geom_cell1 = mesh.insert({ node_1, node_2, node_3 });
    auto geom_cell2 = mesh.insert({ node_2, node_4, node_3 });
    auto geom_cell3 = mesh.insert({ node_4, node_0, node_3 });

    // assert you read 4 cells
    EXPECT_EQ(mesh.nCells(), 4);

    // assert the boundary is made of 4 cells
    EXPECT_EQ(mesh.boundary().nCells(), 4);

    // erase a simplex
    std::cout << "Erasing simplex..." << std::endl;
    mesh.erase(geom_cell0);
    topology.erase(geom_cell0->simplex());

    // assert the mesh has now 3 cells
    EXPECT_EQ(mesh.nCells(), 3);

    // assert the boundary is now made of 5 cells
    EXPECT_EQ(mesh.boundary().nCells(), 5);

    // check that erasing a cell twice does not result in an error
    mesh.erase(geom_cell0);
    topology.erase(geom_cell0->simplex());

    mesh.erase(geom_cell1);
    topology.erase(geom_cell1->simplex());

    // assert the mesh has now 2 cells
    EXPECT_EQ(mesh.nCells(), 2);

    // assert the boundary is now made of 4 cells
    EXPECT_EQ(mesh.boundary().nCells(), 4);
}
