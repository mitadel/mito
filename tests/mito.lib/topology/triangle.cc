#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/topology.h>


TEST(Topology, Triangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // build nodes
    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();
    auto vertex2 = topology.vertex();

    // build triangle with a combination of vertices
    auto tri = topology.triangle({ vertex0, vertex1, vertex2 });
    // check that the vertices of the triangle are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(mito::math::permutation_sign(tri->vertices(), { vertex0, vertex1, vertex2 }), +1);

    // build triangle with a combination of vertices
    tri = topology.triangle({ vertex1, vertex2, vertex0 });
    // check that the vertices of the triangle are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(mito::math::permutation_sign(tri->vertices(), { vertex1, vertex2, vertex0 }), +1);

    // build triangle with a combination of vertices
    tri = topology.triangle({ vertex2, vertex0, vertex1 });
    // check that the vertices of the triangle are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(mito::math::permutation_sign(tri->vertices(), { vertex2, vertex0, vertex1 }), +1);

    // build triangle with a combination of vertices
    tri = topology.triangle({ vertex0, vertex2, vertex1 });
    // check that the vertices of the triangle are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(mito::math::permutation_sign(tri->vertices(), { vertex0, vertex2, vertex1 }), +1);

    // build triangle with a combination of vertices
    tri = topology.triangle({ vertex1, vertex0, vertex2 });
    // check that the vertices of the triangle are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(mito::math::permutation_sign(tri->vertices(), { vertex1, vertex0, vertex2 }), +1);

    // build triangle with a combination of vertices
    tri = topology.triangle({ vertex2, vertex1, vertex0 });
    // check that the vertices of the triangle are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(mito::math::permutation_sign(tri->vertices(), { vertex2, vertex1, vertex0 }), +1);
}


// end of file
