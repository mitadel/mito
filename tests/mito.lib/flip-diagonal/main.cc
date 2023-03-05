#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

TEST(FlipDiagonal, TestFlipDiagonal)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // build vertices
    auto & vertex0 = topology.vertex();
    auto & vertex1 = topology.vertex();
    auto & vertex2 = topology.vertex();
    auto & vertex3 = topology.vertex();

    // build segments
    auto & segment_a = topology.segment({ vertex0, vertex1 });
    auto & segment_b = topology.segment({ vertex1, vertex2 });
    auto & segment_c = topology.segment({ vertex2, vertex3 });
    auto & segment_d = topology.segment({ vertex3, vertex0 });
    auto & segment_e = topology.segment({ vertex0, vertex2 });
    auto & segment_e_flip = topology.flip(segment_e);

    // build triangles
    auto & simplex0 = topology.triangle({ segment_a, segment_b, segment_e_flip });
    auto & simplex1 = topology.triangle({ segment_e, segment_c, segment_d });

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t, 2>(geometry);
    mesh.insert(simplex0);
    mesh.insert(simplex1);

    EXPECT_EQ(mesh.nCells(), 2);

    // flip the common edge of the two triangles
    mito::mesh::flipDiagonal(mesh, simplex0, simplex1);

    EXPECT_EQ(mesh.nCells(), 2);

    // assert that the original diagonal was erased
    EXPECT_EQ(segment_e.references(), 0);
    EXPECT_EQ(segment_e_flip.references(), 0);

    // assert that the new diagonal is now in use (by the factory and by the two triangles)
    auto & segment_f = topology.segment({ vertex1, vertex3 });
    EXPECT_EQ(segment_f->footprint().references(), 3);
}
