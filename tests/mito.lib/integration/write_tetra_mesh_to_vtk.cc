#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(VtkWriter, WriteTetraMeshToVtk)
{
    // a Euclidean coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::EUCLIDEAN>();

    // an empty mesh of simplicial topology in 3D
    auto mesh = mito::mesh::mesh<mito::geometry::tetrahedron_t<3>>();

    // vertices for one single tetrahedron
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.0, 0.0, 1.0 });

    // insert tetrahedron in the mesh
    mesh.insert({ node_0, node_1, node_2, node_3 });

    // use tetra
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system);

    // write mesh to vtk file
    mito::io::vtk::writer("tetra_output", tetra_mesh, coord_system);
}