#include <gtest/gtest.h>
#include <mito/mito.h>

TEST(Mesh, BuildMesh)
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

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<2, mito::topology::simplex_t>();

    auto point0 = point_cloud.point({ 0.0, 0.0 });
    auto point1 = point_cloud.point({ 1.0, 0.0 });
    auto point2 = point_cloud.point({ 1.0, 1.0 });
    auto point3 = point_cloud.point({ 0.5, 0.5 });
    auto point4 = point_cloud.point({ 0.0, 1.0 });

    auto & vertex0 = topology.vertex();
    auto & vertex1 = topology.vertex();
    auto & vertex2 = topology.vertex();
    auto & vertex3 = topology.vertex();
    auto & vertex4 = topology.vertex();

    mesh.insert(vertex0, point0);
    mesh.insert(vertex1, point1);
    mesh.insert(vertex2, point2);
    mesh.insert(vertex3, point3);
    mesh.insert(vertex4, point4);

    auto & segment0 = topology.segment({ vertex0, vertex1 });
    auto & segment1 = topology.segment({ vertex1, vertex3 });
    auto & segment2 = topology.segment({ vertex3, vertex0 });
    auto & cell0 = topology.triangle({ segment0, segment1, segment2 });

    auto & segment3 = topology.segment({ vertex1, vertex2 });
    auto & segment4 = topology.segment({ vertex2, vertex3 });
    auto & segment5 = topology.segment({ vertex3, vertex1 });
    auto & cell1 = topology.triangle({ segment3, segment4, segment5 });

    auto & segment6 = topology.segment({ vertex2, vertex4 });
    auto & segment7 = topology.segment({ vertex4, vertex3 });
    auto & segment8 = topology.segment({ vertex3, vertex2 });
    auto & cell2 = topology.triangle({ segment6, segment7, segment8 });

    auto & segment9 = topology.segment({ vertex4, vertex0 });
    auto & segment10 = topology.segment({ vertex0, vertex3 });
    auto & segment11 = topology.segment({ vertex3, vertex4 });
    auto & cell3 = topology.triangle({ segment9, segment10, segment11 });

    mesh.insert(cell0);
    mesh.insert(cell1);
    mesh.insert(cell2);
    mesh.insert(cell3);

    // assert you read 4 cells
    EXPECT_EQ(mesh.nCells<2>(), 4);

    return;
}

TEST(Mesh, LoadSummitMesh2D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::mesh::summit<2, mito::topology::simplex_t>(fileStream, topology, point_cloud);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;

    t = clock();
    auto boundary_mesh = mesh.boundary();
    std::cout << "Fetched boundary in " << clock() - t << std::endl;
}

TEST(Mesh, LoadSummitMesh3D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("cube.summit");
    auto mesh = mito::mesh::summit<3, mito::topology::simplex_t>(fileStream, topology, point_cloud);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;
}
