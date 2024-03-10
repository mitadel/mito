// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/topology.h>


TEST(Topology, Tetrahedron)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // build nodes
    auto vertex1 = topology.vertex();
    auto vertex2 = topology.vertex();
    auto vertex3 = topology.vertex();
    auto vertex4 = topology.vertex();

    // build tetrahedron with a combination of vertices
    auto tet = topology.tetrahedron({ vertex1, vertex2, vertex3, vertex4 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex1, vertex2, vertex3, vertex4 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex1, vertex3, vertex4, vertex2 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex1, vertex3, vertex4, vertex2 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex1, vertex4, vertex2, vertex3 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex1, vertex4, vertex2, vertex3 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex2, vertex1, vertex4, vertex3 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex2, vertex1, vertex4, vertex3 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex2, vertex3, vertex1, vertex4 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex2, vertex3, vertex1, vertex4 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex2, vertex4, vertex3, vertex1 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex2, vertex4, vertex3, vertex1 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex3, vertex1, vertex2, vertex4 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex3, vertex1, vertex2, vertex4 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex3, vertex2, vertex4, vertex1 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex3, vertex2, vertex4, vertex1 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex3, vertex4, vertex1, vertex2 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex3, vertex4, vertex1, vertex2 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex4, vertex1, vertex3, vertex2 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex4, vertex1, vertex3, vertex2 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex4, vertex2, vertex1, vertex3 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex4, vertex2, vertex1, vertex3 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex4, vertex3, vertex2, vertex1 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex4, vertex3, vertex2, vertex1 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex1, vertex2, vertex4, vertex3 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex1, vertex2, vertex4, vertex3 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex1, vertex3, vertex2, vertex4 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex1, vertex3, vertex2, vertex4 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex1, vertex4, vertex3, vertex2 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex1, vertex4, vertex3, vertex2 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex2, vertex1, vertex3, vertex4 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex2, vertex1, vertex3, vertex4 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex2, vertex3, vertex4, vertex1 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex2, vertex3, vertex4, vertex1 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex2, vertex4, vertex1, vertex3 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex2, vertex4, vertex1, vertex3 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex3, vertex1, vertex4, vertex2 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex3, vertex1, vertex4, vertex2 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex3, vertex2, vertex1, vertex4 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex3, vertex2, vertex1, vertex4 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex3, vertex4, vertex2, vertex1 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex3, vertex4, vertex2, vertex1 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex4, vertex1, vertex2, vertex3 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex4, vertex1, vertex2, vertex3 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex4, vertex2, vertex3, vertex1 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex4, vertex2, vertex3, vertex1 }), +1);

    // build tetrahedron with a combination of vertices
    tet = topology.tetrahedron({ vertex4, vertex3, vertex1, vertex2 });
    // check that the vertices of the tetrahedron are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(
        mito::math::permutation_sign(tet->vertices(), { vertex4, vertex3, vertex1, vertex2 }), +1);
}


// end of file
