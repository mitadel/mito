#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(Tetra, ZeroSubdivisions)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 3D
    auto mesh = mito::mesh::mesh<mito::topology::tetrahedron_t>(geometry);

    // vertices for one single tetrahedron
    auto vertex0 = geometry.node({ 0.0, 0.0, 0.0 });
    auto vertex1 = geometry.node({ 1.0, 0.0, 0.0 });
    auto vertex2 = geometry.node({ 0.0, 1.0, 0.0 });
    auto vertex3 = geometry.node({ 0.0, 0.0, 1.0 });

    // build the tetrahedron
    const auto cell = topology.tetrahedron({ vertex0, vertex1, vertex2, vertex3 });

    // insert cell in the mesh
    mesh.insert(cell);

    // generate tetra mesh with 0 subdivisions
    auto tetra_mesh = tetra(mesh, geometry, 0);

    // assert that the refined mesh has the same number of elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), mesh.nCells());
}
