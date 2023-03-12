// code guard
#if !defined(mito_mesh_tetra_h)
#define mito_mesh_tetra_h


namespace mito::mesh {

    template <class cellT, int D /*spatial dimension*/>
    auto subdivide(
        const vertex_t & vertex_0, const vertex_t & vertex_1, const vertex_t & vertex_2,
        auto & geometry, mesh_t<cellT, D> & subdivided_mesh, int n_refinements) -> void
    requires(std::is_same_v<cellT, mito::topology::triangle_t>)
    {
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

        // if there are no more refinements to be made
        if (n_refinements == 1) {

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

        // otherwise recursively subdivide the four cells
        subdivide(vertex_0, vertex_01, vertex_20, geometry, subdivided_mesh, n_refinements - 1);
        subdivide(vertex_01, vertex_1, vertex_12, geometry, subdivided_mesh, n_refinements - 1);
        subdivide(vertex_12, vertex_2, vertex_20, geometry, subdivided_mesh, n_refinements - 1);
        subdivide(vertex_20, vertex_01, vertex_12, geometry, subdivided_mesh, n_refinements - 1);

        // all done
        return;
    }

    template <class cellT, int D /*spatial dimension*/>
    auto tetra(mesh_t<cellT, D> & mesh, auto & geometry, int n_refinements = 1) -> mesh_t<cellT, D>
    requires(std::is_same_v<cellT, mito::topology::triangle_t>)
    {
        // instantiate a new (empty) mesh for the refined mesh
        mesh_t<cellT, D> subdivided_mesh(geometry);

        // loop on the cells of the mesh
        for (const auto & cell : mesh.cells()) {

            // get the vertices of the cell in the order dictated by the orientation
            auto vertices = mito::topology::vertices(cell);

            // recursively subdivide the cell identified by these three vertices
            subdivide(
                vertices[0], vertices[1], vertices[2], geometry, subdivided_mesh, n_refinements);
        }

        return subdivided_mesh;
    }

}


#endif

// end of file
