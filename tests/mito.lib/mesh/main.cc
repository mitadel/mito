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

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

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

    // add nodes to geometry
    geometry.node(vertex0, point0);
    geometry.node(vertex1, point1);
    geometry.node(vertex2, point2);
    geometry.node(vertex3, point3);
    geometry.node(vertex4, point4);

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
    EXPECT_EQ(mesh.nCells(), 4);

    // assert you found 4 cells (segments) on the boundary
    {
        auto boundary_mesh = mesh.boundary();
        EXPECT_EQ(boundary_mesh.nCells(), 4);
    }

    // TOFIX: 8 vertices are found on the boundary due to repeated entries with different
    //          orientations...
    // assert you found 4 cells (vertices) on the boundary
    {
        auto boundary_mesh = mesh.boundary<0>();
        EXPECT_EQ(boundary_mesh.nCells(), 8);
    }

    return;
}

TEST(Mesh, LoadSummitSegmentsMesh3D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    std::ifstream fileStream("segments.summit");
    auto mesh = mito::mesh::summit<mito::topology::segment_t>(fileStream, geometry);

    // assert you read 10 cells
    EXPECT_EQ(mesh.nCells(), 10);

    // assert you found 2 cells (vertices) on the boundary
    {
        auto boundary_mesh = mesh.boundary();
        EXPECT_EQ(boundary_mesh.nCells(), 2);
    }

    // show me the cells
    std::cout << "Initial mesh: " << std::endl;
    int count = 0;
    mito::topology::segment_t segment;
    for (const auto & simplex : mesh.cells()) {
        ++count;
        if (count == 5) {
            segment = simplex;
        }
    }

    // erase a simplex
    mesh.erase(segment);
    topology.erase(segment);

    // assert you read 9 cells
    EXPECT_EQ(mesh.nCells(), 9);

    // assert you found 4 cells (vertices) on the boundary
    {
        auto boundary_mesh = mesh.boundary();
        EXPECT_EQ(boundary_mesh.nCells(), 4);
    }
}

TEST(Mesh, LoadSummitMesh2D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::mesh::summit<mito::topology::simplex_t<2>>(fileStream, geometry);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;

    t = clock();
    auto boundary_mesh = mesh.boundary();
    std::cout << "Fetched boundary in " << clock() - t << std::endl;
}

// TODO: add test on triangle in 3D

TEST(Mesh, LoadSummitMesh3D)
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
    auto mesh = mito::mesh::summit<mito::topology::simplex_t<3>>(fileStream, geometry);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;
}
