// code guard
#if !defined(mito_mesh_tetra_h)
#define mito_mesh_tetra_h


namespace mito::mesh {

    template <class cellT, int D /*spatial dimension*/>
    auto subdivide(const cellT & cell, auto & geometry, mesh_t<cellT, D> & subdivided_mesh) -> void
    {
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
        subdivided_mesh.insert(new_cell_0);
        subdivided_mesh.insert(new_cell_1);
        subdivided_mesh.insert(new_cell_2);
        subdivided_mesh.insert(new_cell_3);

        // all done
        return;
    }

    template <class cellT, int D /*spatial dimension*/>
    auto tetra(mesh_t<cellT, D> & mesh, auto & geometry) -> mesh_t<cellT, D>
    {
        // instantiate a new (empty) mesh for the refined mesh
        mesh_t<cellT, D> subdivided_mesh(geometry);

        // loop on the cells of the mesh
        for (const auto & cell : mesh.cells()) {
            subdivide(cell, geometry, subdivided_mesh);
        }

        return subdivided_mesh;
    }

}


#endif

// end of file
