#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <mito/io.h>
#include <mito/manifolds.h>


TEST(Tetra, Tetrahedron)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 3D
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::EUCLIDEAN>();

    // an empty mesh of tetrahedra
    auto mesh = mito::mesh::mesh<mito::topology::tetrahedron_t>(geometry);

    // build nodes of a tetrahedron
    auto vertex1 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0, 0.0 });
    auto vertex2 = mito::geometry::node(geometry, coord_system, { 1.0, 0.0, 0.0 });
    auto vertex3 = mito::geometry::node(geometry, coord_system, { 0.0, 1.0, 0.0 });
    auto vertex4 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0, 1.0 });

    // build tetrahedron with a positive volume
    auto tetrahedron = topology.tetrahedron({ vertex1, vertex2, vertex3, vertex4 });

    // insert tetrahedron in mesh
    mesh.insert(tetrahedron);

    // do tetra mesh refinement
    const auto subdivisions = 2;
    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, coord_system, subdivisions);
    // assert that the refined mesh has 8 times more elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), std::pow(8, subdivisions) * mesh.nCells());

    // compute the volume of the original mesh
    auto volume_mesh = mito::manifolds::manifold(mesh, coord_system).volume();

    // compute the volume of the refined mesh
    auto volume_tetra_mesh = mito::manifolds::manifold(tetra_mesh, coord_system).volume();

    // assert that the two volumes coincide
    EXPECT_DOUBLE_EQ(volume_mesh, volume_tetra_mesh);
}
