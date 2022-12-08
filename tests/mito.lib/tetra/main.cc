#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

template <
    int D /*spatial dimension*/, template <int> class cellT, int N /*highest order of simplices */>
auto
tetra(mito::mesh::mesh_t<D, cellT, N> & mesh, auto & topology, auto & point_cloud)
    -> mito::mesh::mesh_t<D, cellT, N>
{
    // instantiate a new mesh for the boundary elements
    mito::mesh::mesh_t<D, cellT, N> tetra_mesh(topology, point_cloud);

    // loop on the cells of the mesh
    for (const auto & cell : mesh.cells()) {

        // get the three vertices
        const auto & edge_0 = cell->composition()[0];
        const auto & edge_1 = cell->composition()[1];
        const auto & vertex_0 = edge_0->composition()[0];
        const auto & vertex_1 = edge_0->composition()[1];
        const auto & vertex_2 = edge_1->composition()[1];

        // insert old nodes in new mesh
        tetra_mesh.insert(vertex_0, mesh.point(vertex_0));
        tetra_mesh.insert(vertex_1, mesh.point(vertex_1));
        tetra_mesh.insert(vertex_2, mesh.point(vertex_2));

        // compute the middle point of the segment 0->1
        auto & vertex_01 = topology.vertex();
        auto point_01 = point_cloud.point(
            0.5 * (mesh.point(vertex_0)->coordinates() + mesh.point(vertex_1)->coordinates()));

        // compute the middle point of the segment 1->2
        auto & vertex_12 = topology.vertex();
        auto point_12 = point_cloud.point(
            0.5 * (mesh.point(vertex_1)->coordinates() + mesh.point(vertex_2)->coordinates()));

        // compute the middle point of the segment 2->0
        auto & vertex_20 = topology.vertex();
        auto point_20 = point_cloud.point(
            0.5 * (mesh.point(vertex_2)->coordinates() + mesh.point(vertex_0)->coordinates()));

        // insert new nodes in new mesh
        tetra_mesh.insert(vertex_01, point_01);
        tetra_mesh.insert(vertex_12, point_12);
        tetra_mesh.insert(vertex_20, point_20);

        // instantiate new cells
        auto & new_cell_0 = topology.triangle({ vertex_0, vertex_01, vertex_20 });
        auto & new_cell_1 = topology.triangle({ vertex_01, vertex_1, vertex_12 });
        auto & new_cell_2 = topology.triangle({ vertex_12, vertex_2, vertex_20 });
        auto & new_cell_3 = topology.triangle({ vertex_20, vertex_01, vertex_12 });

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

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<2, mito::topology::simplex_t>(topology, point_cloud);

    auto point0 = point_cloud.point({ 0.0, 0.0 });
    auto point1 = point_cloud.point({ 1.0, 0.0 });
    auto point2 = point_cloud.point({ 1.0, 1.0 });
    auto point3 = point_cloud.point({ 0.0, 1.0 });

    // build vertices
    auto & vertex0 = topology.vertex();
    auto & vertex1 = topology.vertex();
    auto & vertex2 = topology.vertex();
    auto & vertex3 = topology.vertex();

    // insert in the mesh
    mesh.insert(vertex0, point0);
    mesh.insert(vertex1, point1);
    mesh.insert(vertex2, point2);
    mesh.insert(vertex3, point3);

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

    auto tetra_mesh = tetra(mesh, topology, point_cloud);

    EXPECT_EQ(tetra_mesh.nCells(), 8);
}

TEST(Tetra, MeshRectangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::mesh::summit<2, mito::topology::simplex_t>(fileStream, topology, point_cloud);

    int cells = mesh.nCells();

    auto tetra_mesh = tetra(mesh, topology, point_cloud);

    EXPECT_EQ(tetra_mesh.nCells(), 4 * cells);
}