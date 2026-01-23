// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mesh.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Tetra, Triangle3D)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of triangles in 3D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<3>>();

    // build nodes of a triangle (counterclockwise order)
    auto node_1 = mito::geometry::node(coord_system, { 0.0, 0.0, 1.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });

    // insert triangle in mesh
    mesh.insert({ node_1, node_2, node_3 });

    // do tetra mesh refinements
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, 2);
}
