// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <mito/io.h>
#include <mito/manifolds.h>


TEST(Tetra, Triangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // an empty mesh of triangles
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    // build nodes of a triangle (counterclockwise order)
    auto vertex1 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0 });
    auto vertex2 = mito::geometry::node(geometry, coord_system, { 1.0, 0.0 });
    auto vertex3 = mito::geometry::node(geometry, coord_system, { 0.0, 1.0 });

    // build triangle with a positive volume
    auto triangle = topology.triangle({ vertex1, vertex2, vertex3 });

    // insert triangle in mesh
    mesh.insert(triangle);

    // do tetra mesh refinements
    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, coord_system, 2);

    // compute the volume of the original mesh
    auto volume_mesh = mito::manifolds::manifold(mesh, coord_system).volume();

    // compute the volume of the refined mesh
    auto volume_tetra_mesh = mito::manifolds::manifold(tetra_mesh, coord_system).volume();

    // assert that the two volumes coincide
    EXPECT_DOUBLE_EQ(volume_mesh, volume_tetra_mesh);
}
