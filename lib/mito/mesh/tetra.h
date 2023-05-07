// code guard
#if !defined(mito_mesh_tetra_h)
#define mito_mesh_tetra_h


namespace mito::mesh {

    template <class cellT, int D /*spatial dimension*/>
    auto subdivide(
        const vertex_t & vertex_0, const vertex_t & vertex_1,
        mito::geometry::geometry_t<D> & geometry, mesh_t<cellT, D> & subdivided_mesh,
        int n_refinements) -> void
    requires(std::is_same_v<cellT, mito::topology::segment_t>)
    {
        if (n_refinements == 0) {
            auto & new_cell = geometry.topology().segment({ vertex_0, vertex_1 });

            // insert new cells in new mesh
            subdivided_mesh.insert(new_cell);

            return;
        }

        // compute the middle point of the segment 0->1
        auto & vertex_01 = geometry.node(
            0.5
            * (geometry.point(vertex_0)->coordinates() + geometry.point(vertex_1)->coordinates()));

        // if there are no more refinements to be made
        if (n_refinements == 1) {

            // instantiate new cells
            auto & new_cell_0 = geometry.topology().segment({ vertex_0, vertex_01 });
            auto & new_cell_1 = geometry.topology().segment({ vertex_01, vertex_1 });

            // insert new cells in new mesh
            subdivided_mesh.insert(new_cell_0);
            subdivided_mesh.insert(new_cell_1);

            // all done
            return;
        }

        // otherwise recursively subdivide the four cells
        subdivide(vertex_0, vertex_01, geometry, subdivided_mesh, n_refinements - 1);
        subdivide(vertex_01, vertex_1, geometry, subdivided_mesh, n_refinements - 1);

        // all done
        return;
    }

    template <class cellT, int D /*spatial dimension*/>
    auto subdivide(
        const vertex_t & vertex_0, const vertex_t & vertex_1, const vertex_t & vertex_2,
        mito::geometry::geometry_t<D> & geometry, mesh_t<cellT, D> & subdivided_mesh,
        int n_refinements) -> void
    requires(std::is_same_v<cellT, mito::topology::triangle_t>)
    {
        if (n_refinements == 0) {
            auto & new_cell = geometry.topology().triangle({ vertex_0, vertex_1, vertex_2 });

            // insert new cells in new mesh
            subdivided_mesh.insert(new_cell);

            return;
        }

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
    auto subdivide(
        const vertex_t & vertex_0, const vertex_t & vertex_1, const vertex_t & vertex_2,
        const vertex_t & vertex_3, mito::geometry::geometry_t<D> & geometry,
        mesh_t<cellT, D> & subdivided_mesh, int n_refinements) -> void
    requires(std::is_same_v<cellT, mito::topology::tetrahedron_t>)
    {
        if (n_refinements == 0) {
            auto & new_cell =
                geometry.topology().tetrahedron({ vertex_0, vertex_1, vertex_2, vertex_3 });

            // insert new cells in new mesh
            subdivided_mesh.insert(new_cell);

            return;
        }

        // compute the middle point of the segment 0->1
        auto & vertex_01 = geometry.node(
            0.5
            * (geometry.point(vertex_0)->coordinates() + geometry.point(vertex_1)->coordinates()));

        // compute the middle point of the segment 0->2
        auto & vertex_02 = geometry.node(
            0.5
            * (geometry.point(vertex_0)->coordinates() + geometry.point(vertex_2)->coordinates()));

        // compute the middle point of the segment 0->3
        auto & vertex_03 = geometry.node(
            0.5
            * (geometry.point(vertex_0)->coordinates() + geometry.point(vertex_3)->coordinates()));

        // compute the middle point of the segment 1->2
        auto & vertex_12 = geometry.node(
            0.5
            * (geometry.point(vertex_1)->coordinates() + geometry.point(vertex_2)->coordinates()));

        // compute the middle point of the segment 1->3
        auto & vertex_13 = geometry.node(
            0.5
            * (geometry.point(vertex_1)->coordinates() + geometry.point(vertex_3)->coordinates()));

        // compute the middle point of the segment 2->3
        auto & vertex_23 = geometry.node(
            0.5
            * (geometry.point(vertex_2)->coordinates() + geometry.point(vertex_3)->coordinates()));

        // if there are no more refinements to be made
        if (n_refinements == 1) {

            // instantiate new cells
            auto & new_cell_0 =
                geometry.topology().tetrahedron({ vertex_0, vertex_01, vertex_02, vertex_03 });
            auto & new_cell_1 =
                geometry.topology().tetrahedron({ vertex_1, vertex_01, vertex_13, vertex_12 });
            auto & new_cell_2 =
                geometry.topology().tetrahedron({ vertex_2, vertex_12, vertex_02, vertex_23 });
            auto & new_cell_3 =
                geometry.topology().tetrahedron({ vertex_3, vertex_03, vertex_13, vertex_23 });
            auto & new_cell_4 =
                geometry.topology().tetrahedron({ vertex_02, vertex_01, vertex_13, vertex_03 });
            auto & new_cell_5 =
                geometry.topology().tetrahedron({ vertex_02, vertex_03, vertex_13, vertex_23 });
            auto & new_cell_6 =
                geometry.topology().tetrahedron({ vertex_02, vertex_01, vertex_13, vertex_12 });
            auto & new_cell_7 =
                geometry.topology().tetrahedron({ vertex_02, vertex_23, vertex_13, vertex_12 });

            // insert new cells in new mesh
            subdivided_mesh.insert(new_cell_0);
            subdivided_mesh.insert(new_cell_1);
            subdivided_mesh.insert(new_cell_2);
            subdivided_mesh.insert(new_cell_3);
            subdivided_mesh.insert(new_cell_4);
            subdivided_mesh.insert(new_cell_5);
            subdivided_mesh.insert(new_cell_6);
            subdivided_mesh.insert(new_cell_7);

            // all done
            return;
        }

        // otherwise recursively subdivide the four cells
        subdivide(
            vertex_0, vertex_01, vertex_02, vertex_03, geometry, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_1, vertex_01, vertex_13, vertex_12, geometry, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_2, vertex_12, vertex_02, vertex_23, geometry, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_3, vertex_03, vertex_13, vertex_23, geometry, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_02, vertex_01, vertex_13, vertex_03, geometry, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_02, vertex_03, vertex_13, vertex_23, geometry, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_02, vertex_01, vertex_13, vertex_12, geometry, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_02, vertex_23, vertex_13, vertex_12, geometry, subdivided_mesh,
            n_refinements - 1);

        // all done
        return;
    }

    template <class cellT, int D /*spatial dimension*/>
    auto tetra(
        mesh_t<cellT, D> & mesh, mito::geometry::geometry_t<D> & geometry, int n_refinements = 1)
        -> mesh_t<cellT, D>
    {
        // instantiate a new (empty) mesh for the refined mesh
        mesh_t<cellT, D> subdivided_mesh(geometry);

        // loop on the cells of the mesh
        for (const auto & cell : mesh.cells()) {

            // get the vertices of the cell in the order dictated by the orientation
            auto vertices = mito::topology::vertices(cell);

            // helper function to expand array to parameter pack
            constexpr auto _subdivide =
                []<size_t... J>(
                    const mito::topology::vertex_simplex_composition_t<cellT::resource_t::order> &
                        vertices,
                    mito::geometry::geometry_t<D> & geometry, mesh_t<cellT, D> & subdivided_mesh,
                    int n_refinements, std::index_sequence<J...>) {
                    return subdivide(vertices[J]..., geometry, subdivided_mesh, n_refinements);
                };

            // recursively subdivide the cell identified by these vertices
            _subdivide(
                vertices, geometry, subdivided_mesh, n_refinements,
                std::make_index_sequence<cellT::resource_t::order + 1> {});
        }

        return subdivided_mesh;
    }

}


#endif

// end of file
