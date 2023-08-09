#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(SummitReader, LoadSummitSegmentsMesh3D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    std::ifstream fileStream("segments.summit");
    auto mesh = mito::io::summit::reader<mito::topology::segment_t>(fileStream, geometry);

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

    // assert you read 9 cells
    EXPECT_EQ(mesh.nCells(), 9);

    // assert you found 4 cells (vertices) on the boundary
    {
        auto boundary_mesh = mesh.boundary();
        EXPECT_EQ(boundary_mesh.nCells(), 4);
    }
}