// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/manifolds.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Manifolds, Tetrahedron3D)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of tetrahedra
    auto mesh = mito::mesh::mesh<mito::geometry::tetrahedron_t<3>>();

    // create a manifold on {mesh} with Euclidean metric
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // build nodes
    auto node_1 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });
    auto node_4 = mito::geometry::node(coord_system, { 0.0, 0.0, 1.0 });

    // build tetrahedron with a positive volume (reference tetrahedron)
    auto tetrahedron = mito::geometry::tetrahedron<3>({ node_1, node_2, node_3, node_4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_1, node_2, node_3, node_4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_1, node_3, node_4, node_2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_1, node_4, node_2, node_3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_2, node_1, node_4, node_3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_2, node_3, node_1, node_4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_2, node_4, node_3, node_1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_3, node_1, node_2, node_4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_3, node_2, node_4, node_1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_3, node_4, node_1, node_2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_4, node_1, node_3, node_2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_4, node_2, node_1, node_3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an even permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_4, node_3, node_2, node_1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), 1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_1, node_2, node_4, node_3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_1, node_3, node_2, node_4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_1, node_4, node_3, node_2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_2, node_1, node_3, node_4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_2, node_3, node_4, node_1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_2, node_4, node_1, node_3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_3, node_1, node_4, node_2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_3, node_2, node_1, node_4 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_3, node_4, node_2, node_1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_4, node_1, node_2, node_3 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_4, node_2, node_3, node_1 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);

    // create a tetrahedron from an odd permutation of the vertices with respect to the reference
    tetrahedron = mito::geometry::tetrahedron<3>({ node_4, node_3, node_1, node_2 });
    // check that the volume of tetrahedron is correct
    EXPECT_DOUBLE_EQ(manifold.volume(tetrahedron), -1.0 / 6.0);
}


// end of file
