// code guard
#if !defined(mito_mesh_tetra_h)
#define mito_mesh_tetra_h


namespace mito::mesh {

    template <int D, geometry::CoordinateType coordT>
    auto midnode(
        const topology::vertex_t & vertex_a, const topology::vertex_t & vertex_b,
        geometry::geometry_t<D> & geometry,
        geometry::coordinate_system_t<D, coordT> & coordinate_system) -> topology::vertex_t
    {
        // return a new node at the midpoint between {vertex_a} and {vertex_b}
        return mito::geometry::node(
            geometry, coordinate_system,
            coordinate_system.midpoint(geometry.point(vertex_a), geometry.point(vertex_b)));
    }

    template <int D /*spatial dimension*/, geometry::CoordinateType coordT>
    auto subdivide(
        const topology::vertex_t & vertex_0, const topology::vertex_t & vertex_1,
        geometry::geometry_t<D> & geometry,
        geometry::coordinate_system_t<D, coordT> & coordinate_system,
        mesh_t<topology::segment_t, D> & subdivided_mesh, int n_refinements) -> void
    {
        // compute the middle point of the segment 0->1
        auto vertex_01 = midnode(vertex_0, vertex_1, geometry, coordinate_system);

        // if there are no more refinements to be made
        if (n_refinements == 1) {

            // TOFIX: don't like geometry having a r/w reference to topology
            // instantiate new cells
            auto new_cell_0 = geometry.topology().segment({ vertex_0, vertex_01 });
            auto new_cell_1 = geometry.topology().segment({ vertex_01, vertex_1 });

            // insert new cells in new mesh
            subdivided_mesh.insert(new_cell_0);
            subdivided_mesh.insert(new_cell_1);

            // all done
            return;
        }

        // otherwise recursively subdivide the four cells
        subdivide(
            vertex_0, vertex_01, geometry, coordinate_system, subdivided_mesh, n_refinements - 1);
        subdivide(
            vertex_01, vertex_1, geometry, coordinate_system, subdivided_mesh, n_refinements - 1);

        // all done
        return;
    }

    template <int D /*spatial dimension*/, geometry::CoordinateType coordT>
    auto subdivide(
        const topology::vertex_t & vertex_0, const topology::vertex_t & vertex_1,
        const topology::vertex_t & vertex_2, geometry::geometry_t<D> & geometry,
        geometry::coordinate_system_t<D, coordT> & coordinate_system,
        mesh_t<topology::triangle_t, D> & subdivided_mesh, int n_refinements) -> void
    {
        // compute the middle point of the segment 0->1
        auto vertex_01 = midnode(vertex_0, vertex_1, geometry, coordinate_system);

        // compute the middle point of the segment 1->2
        auto vertex_12 = midnode(vertex_1, vertex_2, geometry, coordinate_system);

        // compute the middle point of the segment 2->0
        auto vertex_20 = midnode(vertex_2, vertex_0, geometry, coordinate_system);

        // if there are no more refinements to be made
        if (n_refinements == 1) {

            // instantiate new cells
            auto new_cell_0 = geometry.topology().triangle({ vertex_0, vertex_01, vertex_20 });
            auto new_cell_1 = geometry.topology().triangle({ vertex_01, vertex_1, vertex_12 });
            auto new_cell_2 = geometry.topology().triangle({ vertex_12, vertex_2, vertex_20 });
            auto new_cell_3 = geometry.topology().triangle({ vertex_20, vertex_01, vertex_12 });

            // insert new cells in new mesh
            subdivided_mesh.insert(new_cell_0);
            subdivided_mesh.insert(new_cell_1);
            subdivided_mesh.insert(new_cell_2);
            subdivided_mesh.insert(new_cell_3);

            // all done
            return;
        }

        // otherwise recursively subdivide the four cells
        subdivide(
            vertex_0, vertex_01, vertex_20, geometry, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_01, vertex_1, vertex_12, geometry, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_12, vertex_2, vertex_20, geometry, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_20, vertex_01, vertex_12, geometry, coordinate_system, subdivided_mesh,
            n_refinements - 1);

        // all done
        return;
    }

    template <int D /*spatial dimension*/, geometry::CoordinateType coordT>
    auto subdivide(
        const topology::vertex_t & vertex_0, const topology::vertex_t & vertex_1,
        const topology::vertex_t & vertex_2, const topology::vertex_t & vertex_3,
        geometry::geometry_t<D> & geometry,
        geometry::coordinate_system_t<D, coordT> & coordinate_system,
        mesh_t<topology::tetrahedron_t, D> & subdivided_mesh, int n_refinements) -> void
    {
        // compute the middle point of the segment 0->1
        auto vertex_01 = midnode(vertex_0, vertex_1, geometry, coordinate_system);

        // compute the middle point of the segment 0->2
        auto vertex_02 = midnode(vertex_0, vertex_2, geometry, coordinate_system);

        // compute the middle point of the segment 0->3
        auto vertex_03 = midnode(vertex_0, vertex_3, geometry, coordinate_system);

        // compute the middle point of the segment 1->2
        auto vertex_12 = midnode(vertex_1, vertex_2, geometry, coordinate_system);

        // compute the middle point of the segment 1->3
        auto vertex_13 = midnode(vertex_1, vertex_3, geometry, coordinate_system);

        // compute the middle point of the segment 2->3
        auto vertex_23 = midnode(vertex_2, vertex_3, geometry, coordinate_system);

        // if there are no more refinements to be made
        if (n_refinements == 1) {

            // instantiate new cells
            auto new_cell_0 =
                geometry.topology().tetrahedron({ vertex_0, vertex_01, vertex_02, vertex_03 });
            auto new_cell_1 =
                geometry.topology().tetrahedron({ vertex_1, vertex_01, vertex_13, vertex_12 });
            auto new_cell_2 =
                geometry.topology().tetrahedron({ vertex_2, vertex_02, vertex_12, vertex_23 });
            auto new_cell_3 =
                geometry.topology().tetrahedron({ vertex_3, vertex_13, vertex_03, vertex_23 });
            auto new_cell_4 =
                geometry.topology().tetrahedron({ vertex_02, vertex_01, vertex_13, vertex_03 });
            auto new_cell_5 =
                geometry.topology().tetrahedron({ vertex_02, vertex_03, vertex_13, vertex_23 });
            auto new_cell_6 =
                geometry.topology().tetrahedron({ vertex_02, vertex_13, vertex_01, vertex_12 });
            auto new_cell_7 =
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
            vertex_0, vertex_01, vertex_02, vertex_03, geometry, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_1, vertex_01, vertex_13, vertex_12, geometry, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_2, vertex_02, vertex_12, vertex_23, geometry, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_03, vertex_3, vertex_13, vertex_23, geometry, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            vertex_02, vertex_01, vertex_13, vertex_03, geometry, coordinate_system,
            subdivided_mesh, n_refinements - 1);
        subdivide(
            vertex_02, vertex_03, vertex_13, vertex_23, geometry, coordinate_system,
            subdivided_mesh, n_refinements - 1);
        subdivide(
            vertex_01, vertex_02, vertex_13, vertex_12, geometry, coordinate_system,
            subdivided_mesh, n_refinements - 1);
        subdivide(
            vertex_02, vertex_23, vertex_13, vertex_12, geometry, coordinate_system,
            subdivided_mesh, n_refinements - 1);

        // all done
        return;
    }

    template <class cellT, int D /*spatial dimension*/, geometry::CoordinateType coordT>
    auto tetra(
        const mesh_t<cellT, D> & mesh, geometry::geometry_t<D> & geometry,
        geometry::coordinate_system_t<D, coordT> & coordinate_system, int n_refinements = 1)
        -> mesh_t<cellT, D>
    {
        // instantiate a new (empty) mesh for the refined mesh
        mesh_t<cellT, D> subdivided_mesh(geometry);

        // trivial case (just return a copy of the original mesh)
        if (n_refinements == 0) {

            // loop on the cells of the mesh
            for (const auto & cell : mesh.cells()) {

                // insert new cells in new mesh
                subdivided_mesh.insert(cell);

                // all done
                return subdivided_mesh;
            }
        }

        // loop on the cells of the mesh
        for (const auto & cell : mesh.cells()) {

            // get the vertices of the cell in the order dictated by the orientation
            auto vertices = cell->vertices();

            // helper function to expand array to parameter pack
            constexpr auto _subdivide =
                []<size_t... J>(
                    const topology::vertex_simplex_composition_t<topology::order<cellT>()> &
                        vertices,
                    geometry::geometry_t<D> & geometry,
                    geometry::coordinate_system_t<D, coordT> & coordinate_system,
                    mesh_t<cellT, D> & subdivided_mesh, int n_refinements,
                    std::index_sequence<J...>) {
                    return subdivide(
                        vertices[J]..., geometry, coordinate_system, subdivided_mesh,
                        n_refinements);
                };

            // recursively subdivide the cell identified by these vertices
            _subdivide(
                vertices, geometry, coordinate_system, subdivided_mesh, n_refinements,
                std::make_index_sequence<topology::order<cellT>() + 1> {});
        }

        // return the refined mesh
        return subdivided_mesh;
    }

}


#endif

// end of file
