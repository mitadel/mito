#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <stack>

TEST(EraseElement, TestEraseElementMesh)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // ask the geometry for new nodes (this instantiates a new vertex and attaches it to the point)
    auto vertex0 = geometry.node({ 0.0, 0.0 });
    // or equivalently
    // auto & vertex0 = topology.vertex();
    // auto point0 = point_cloud.point({ 0.0, 0.0 });
    // geometry.node(vertex0, point0);
    auto vertex1 = geometry.node({ 1.0, 0.0 });
    auto vertex2 = geometry.node({ 1.0, 1.0 });
    auto vertex3 = geometry.node({ 0.5, 0.5 });
    auto vertex4 = geometry.node({ 0.0, 1.0 });

    // ask the topology for segments connecting the vertices of the nodes above
    auto segment0 = topology.segment({ vertex0, vertex1 });
    auto segment1 = topology.segment({ vertex1, vertex3 });
    auto segment2 = topology.segment({ vertex3, vertex0 });
    auto cell0 = topology.triangle({ segment0, segment1, segment2 });

    auto segment3 = topology.segment({ vertex1, vertex2 });
    auto segment4 = topology.segment({ vertex2, vertex3 });
    auto segment5 = topology.segment({ vertex3, vertex1 });
    auto cell1 = topology.triangle({ segment3, segment4, segment5 });

    auto segment6 = topology.segment({ vertex2, vertex4 });
    auto segment7 = topology.segment({ vertex4, vertex3 });
    auto segment8 = topology.segment({ vertex3, vertex2 });
    auto cell2 = topology.triangle({ segment6, segment7, segment8 });

    auto segment9 = topology.segment({ vertex4, vertex0 });
    auto segment10 = topology.segment({ vertex0, vertex3 });
    auto segment11 = topology.segment({ vertex3, vertex4 });
    auto cell3 = topology.triangle({ segment9, segment10, segment11 });

    // an empty mesh of triangles in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);
    mesh.insert(cell0);
    mesh.insert(cell1);
    mesh.insert(cell2);
    mesh.insert(cell3);

    // assert you read 4 cells
    EXPECT_EQ(mesh.nCells(), 4);

    // assert the boundary is made of 4 cells
    EXPECT_EQ(mesh.boundary().nCells(), 4);

    // show me the cells
    // std::cout << "Initial mesh: " << std::endl;
    // for (const auto & simplex : mesh.cells<2>())
    // {
    //     std::cout << simplex << std::endl;
    // }

    // erase a simplex
    std::cout << "Erasing simplex..." << std::endl;
    mesh.erase(cell0);

    // std::cout << "After erase: " << std::endl;
    // for (const auto & simplex : mesh.cells<2>()) {
    //     std::cout << simplex << std::endl;
    // }

    // assert the mesh has now 3 cells
    EXPECT_EQ(mesh.nCells(), 3);

    // assert the boundary is now made of 5 cells
    EXPECT_EQ(mesh.boundary().nCells(), 5);

    // show me the boundary cells
    // std::cout << "Boundary: " << std::endl;
    // for (const auto & simplex : mito::mesh::boundary<1>(mesh, topology)) {
    //     std::cout << simplex << std::endl;
    // }

    mesh.erase(cell1);

    // assert the mesh has now 2 cells
    EXPECT_EQ(mesh.nCells(), 2);

    // assert the boundary is now made of 4 cells
    EXPECT_EQ(mesh.boundary().nCells(), 4);
}

TEST(EraseElement, TestEraseElementTopology)
{
    /**
     * Mesh with four cells:
    (0,1)           (1,1)
      4               2
      +---------------+
      | .           . |
      |   .       .   |
      |     .   .     |
      |       . 3     |
      |     .   .     |
      |   .       .   |
      | .           . |
      +---------------+
      0               1
    (0,0)           (1,0)
    */

    // an empty topology
    auto & topology = mito::topology::topology();

    auto vertex_0 = topology.vertex();
    auto vertex_1 = topology.vertex();
    auto vertex_2 = topology.vertex();
    auto vertex_3 = topology.vertex();
    auto vertex_4 = topology.vertex();

    // the pile of ids of cells to be erased
    std::stack<mito::topology::triangle_id_t> pile;
    {
        auto segment_0 = topology.segment({ vertex_0, vertex_1 });
        auto segment_1 = topology.segment({ vertex_1, vertex_3 });
        auto segment_2 = topology.segment({ vertex_3, vertex_0 });
        auto cell_0 = topology.triangle({ segment_0, segment_1, segment_2 });

        auto segment_3 = topology.segment({ vertex_1, vertex_2 });
        auto segment_4 = topology.segment({ vertex_2, vertex_3 });
        auto segment_5 = topology.segment({ vertex_3, vertex_1 });
        auto cell_1 = topology.triangle({ segment_3, segment_4, segment_5 });

        auto segment_6 = topology.segment({ vertex_2, vertex_4 });
        auto segment_7 = topology.segment({ vertex_4, vertex_3 });
        auto segment_8 = topology.segment({ vertex_3, vertex_2 });
        topology.triangle({ segment_6, segment_7, segment_8 });

        auto segment_9 = topology.segment({ vertex_4, vertex_0 });
        auto segment_10 = topology.segment({ vertex_0, vertex_3 });
        auto segment_11 = topology.segment({ vertex_3, vertex_4 });
        topology.triangle({ segment_9, segment_10, segment_11 });

        // push the id cells {cell_1} and {cell_0} to the pile of cells to be erased
        pile.push(cell_1.id());
        pile.push(cell_0.id());
    }
    // assert that a segment connecting vertex 1 and 3 exists in the topology (namely, {segment_1})
    EXPECT_TRUE(topology.exists({ vertex_1, vertex_3 }));
    // assert that a segment connecting vertex 3 and 0 exists in the topology (namely, {segment_2})
    EXPECT_TRUE(topology.exists({ vertex_3, vertex_0 }));
    // assert that a segment connecting vertex 2 and 3 exists in the topology (namely, {segment_4})
    EXPECT_TRUE(topology.exists({ vertex_2, vertex_3 }));

    // erase the top cell of the pile (namely, the cell with {segment_0, segment_1, segment_2})
    topology.erase<2>(pile.top());
    // pop the erased cell id
    pile.pop();

    // assert that a segment connecting vertex 1 and 3 no longer exists in the topology
    // ({segment_1} was erased because it is unused after erasing {cell_0})
    EXPECT_FALSE(topology.exists({ vertex_1, vertex_3 }));
    // assert that a segment connecting vertex 3 and 0 no longer exists in the topology
    // ({segment_2} was also erased because unused after erasing {cell_0})
    EXPECT_FALSE(topology.exists({ vertex_3, vertex_0 }));

    // erase the top cell of the pile (namely, the cell with {segment_0, segment_1, segment_2})
    topology.erase<2>(pile.top());
    // pop the erased cell id
    pile.pop();
    // assert that a segment connecting vertex 2 and 3 no longer exists in the topology
    // ({segment_4} was erased because it is unused after erasing {cell_1})
    EXPECT_FALSE(topology.exists({ vertex_2, vertex_3 }));
}