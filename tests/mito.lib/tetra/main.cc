#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

template <class cellT, int D /*spatial dimension*/>
auto
tetra(mito::mesh::mesh_t<cellT, D> & mesh, auto & geometry) -> mito::mesh::mesh_t<cellT, D>
{
    // instantiate a new (empty) mesh for the refined mesh
    mito::mesh::mesh_t<cellT, D> tetra_mesh(geometry);

    // loop on the cells of the mesh
    for (const auto & cell : mesh.cells()) {

        // get the three vertices
        const auto & edge_0 = cell->composition()[0];
        const auto & edge_1 = cell->composition()[1];
        const auto & vertex_0 = edge_0->composition()[0]->footprint();
        const auto & vertex_1 = edge_0->composition()[1]->footprint();
        const auto & vertex_2 = edge_1->composition()[1]->footprint();

        // compute the middle point of the segment 0->1
        auto & vertex_01 = geometry.node(
            0.5
            * (geometry.point(vertex_0)->coordinates() + geometry.point(vertex_1)->coordinates()));

        // compute the middle point of the segment 1->2
        auto & vertex_12 = geometry.node(
            0.5
            * (geometry.point(vertex_1)->coordinates() + geometry.point(vertex_2)->coordinates()));

        // compute the middle point of the segment 2->0
        auto & vertex_20 = geometry.node(
            0.5
            * (geometry.point(vertex_2)->coordinates() + geometry.point(vertex_0)->coordinates()));

        // instantiate new cells
        auto & new_cell_0 = geometry.topology().triangle({ vertex_0, vertex_01, vertex_20 });
        auto & new_cell_1 = geometry.topology().triangle({ vertex_01, vertex_1, vertex_12 });
        auto & new_cell_2 = geometry.topology().triangle({ vertex_12, vertex_2, vertex_20 });
        auto & new_cell_3 = geometry.topology().triangle({ vertex_20, vertex_01, vertex_12 });

        // insert new cells in new mesh
        tetra_mesh.insert(new_cell_0);
        tetra_mesh.insert(new_cell_1);
        tetra_mesh.insert(new_cell_2);
        tetra_mesh.insert(new_cell_3);
    }

    return tetra_mesh;
}

TEST(Tetra, MeshTwoTriangles)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    // build nodes
    auto & vertex0 = geometry.node({ 0.0, 0.0 });
    auto & vertex1 = geometry.node({ 1.0, 0.0 });
    auto & vertex2 = geometry.node({ 1.0, 1.0 });
    auto & vertex3 = geometry.node({ 0.0, 1.0 });

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

    // insert cells in the mesh
    mesh.insert(simplex0);
    mesh.insert(simplex1);

    EXPECT_EQ(mesh.nCells(), 2);

    auto tetra_mesh = tetra(mesh, geometry);

    EXPECT_EQ(tetra_mesh.nCells(), 8);
}

TEST(Tetra, MeshRectangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::mesh::summit<mito::topology::triangle_t>(fileStream, geometry);

    int cells = mesh.nCells();

    auto tetra_mesh = tetra(mesh, geometry);

    EXPECT_EQ(tetra_mesh.nCells(), 4 * cells);
}