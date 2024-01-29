// code guard
#if !defined(mito_mesh_tetra_h)
#define mito_mesh_tetra_h


namespace mito::mesh {

    template <int D, geometry::CoordinateType coordT>
    auto midnode(
        const geometry::node_t<D> & node_a, const geometry::node_t<D> & node_b,
        geometry::coordinate_system_t<D, coordT> & coordinate_system) -> geometry::node_t<D>
    {
        // return a new node at the midpoint between {node_a} and {node_b}
        return mito::geometry::node(
            coordinate_system, coordinate_system.midpoint(node_a.point(), node_b.point()));
    }

    template <int D /*spatial dimension*/, geometry::CoordinateType coordT>
    auto subdivide(
        const geometry::node_t<D> & node_0, const geometry::node_t<D> & node_1,
        geometry::coordinate_system_t<D, coordT> & coordinate_system,
        mesh_t<geometry::segment_t<D>> & subdivided_mesh, int n_refinements) -> void
    {
        // compute the middle node of the segment 0->1
        auto node_01 = midnode(node_0, node_1, coordinate_system);

        // if there are no more refinements to be made
        if (n_refinements == 1) {

            // insert new cells in new mesh
            subdivided_mesh.insert({ node_0, node_01 });
            subdivided_mesh.insert({ node_01, node_1 });

            // all done
            return;
        }

        // otherwise recursively subdivide the four cells
        subdivide(node_0, node_01, coordinate_system, subdivided_mesh, n_refinements - 1);
        subdivide(node_01, node_1, coordinate_system, subdivided_mesh, n_refinements - 1);

        // all done
        return;
    }

    template <int D /*spatial dimension*/, geometry::CoordinateType coordT>
    auto subdivide(
        const geometry::node_t<D> & node_0, const geometry::node_t<D> & node_1,
        const geometry::node_t<D> & node_2,
        geometry::coordinate_system_t<D, coordT> & coordinate_system,
        mesh_t<geometry::triangle_t<D>> & subdivided_mesh, int n_refinements) -> void
    {
        // compute the middle point of the segment 0->1
        auto node_01 = midnode(node_0, node_1, coordinate_system);

        // compute the middle point of the segment 1->2
        auto node_12 = midnode(node_1, node_2, coordinate_system);

        // compute the middle point of the segment 2->0
        auto node_20 = midnode(node_2, node_0, coordinate_system);

        // if there are no more refinements to be made
        if (n_refinements == 1) {

            // insert new cells in new mesh
            subdivided_mesh.insert({ node_0, node_01, node_20 });
            subdivided_mesh.insert({ node_01, node_1, node_12 });
            subdivided_mesh.insert({ node_12, node_2, node_20 });
            subdivided_mesh.insert({ node_20, node_01, node_12 });

            // all done
            return;
        }

        // otherwise recursively subdivide the four cells
        subdivide(node_0, node_01, node_20, coordinate_system, subdivided_mesh, n_refinements - 1);
        subdivide(node_01, node_1, node_12, coordinate_system, subdivided_mesh, n_refinements - 1);
        subdivide(node_12, node_2, node_20, coordinate_system, subdivided_mesh, n_refinements - 1);
        subdivide(node_20, node_01, node_12, coordinate_system, subdivided_mesh, n_refinements - 1);

        // all done
        return;
    }

    template <int D /*spatial dimension*/, geometry::CoordinateType coordT>
    auto subdivide(
        const geometry::node_t<D> & node_0, const geometry::node_t<D> & node_1,
        const geometry::node_t<D> & node_2, const geometry::node_t<D> & node_3,
        geometry::coordinate_system_t<D, coordT> & coordinate_system,
        mesh_t<geometry::tetrahedron_t<D>> & subdivided_mesh, int n_refinements) -> void
    {
        // compute the middle point of the segment 0->1
        auto node_01 = midnode(node_0, node_1, coordinate_system);

        // compute the middle point of the segment 0->2
        auto node_02 = midnode(node_0, node_2, coordinate_system);

        // compute the middle point of the segment 0->3
        auto node_03 = midnode(node_0, node_3, coordinate_system);

        // compute the middle point of the segment 1->2
        auto node_12 = midnode(node_1, node_2, coordinate_system);

        // compute the middle point of the segment 1->3
        auto node_13 = midnode(node_1, node_3, coordinate_system);

        // compute the middle point of the segment 2->3
        auto node_23 = midnode(node_2, node_3, coordinate_system);

        // if there are no more refinements to be made
        if (n_refinements == 1) {

            // insert new cells in new mesh
            subdivided_mesh.insert({ node_0, node_01, node_02, node_03 });
            subdivided_mesh.insert({ node_1, node_01, node_13, node_12 });
            subdivided_mesh.insert({ node_2, node_02, node_12, node_23 });
            subdivided_mesh.insert({ node_3, node_13, node_03, node_23 });
            subdivided_mesh.insert({ node_02, node_01, node_13, node_03 });
            subdivided_mesh.insert({ node_02, node_03, node_13, node_23 });
            subdivided_mesh.insert({ node_02, node_13, node_01, node_12 });
            subdivided_mesh.insert({ node_02, node_23, node_13, node_12 });

            // all done
            return;
        }

        // otherwise recursively subdivide the four cells
        subdivide(
            node_0, node_01, node_02, node_03, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            node_1, node_01, node_13, node_12, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            node_2, node_02, node_12, node_23, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            node_03, node_3, node_13, node_23, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            node_02, node_01, node_13, node_03, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            node_02, node_03, node_13, node_23, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            node_01, node_02, node_13, node_12, coordinate_system, subdivided_mesh,
            n_refinements - 1);
        subdivide(
            node_02, node_23, node_13, node_12, coordinate_system, subdivided_mesh,
            n_refinements - 1);

        // all done
        return;
    }

    template <class cellT, int D /*spatial dimension*/, geometry::CoordinateType coordT>
    auto tetra(
        const mesh_t<cellT> & mesh, geometry::coordinate_system_t<D, coordT> & coordinate_system,
        int n_refinements = 1) -> mesh_t<cellT>
    {
        // instantiate a new (empty) mesh for the refined mesh
        mesh_t<cellT> subdivided_mesh;

        // trivial case (just return a copy of the original mesh)
        if (n_refinements == 0) {

            // loop on the cells of the mesh
            for (const auto & cell : mesh.cells()) {

                // insert new cells in new mesh
                subdivided_mesh.insert(cell.nodes());

                // all done
                return subdivided_mesh;
            }
        }

        // loop on the cells of the mesh
        for (const auto & cell : mesh.cells()) {

            // get the nodes of the cell in the order dictated by the orientation
            auto nodes = cell.nodes();

            // helper function to expand array to parameter pack
            constexpr auto _subdivide =
                []<size_t... J>(
                    const auto & nodes,
                    geometry::coordinate_system_t<D, coordT> & coordinate_system,
                    mesh_t<cellT> & subdivided_mesh, int n_refinements, std::index_sequence<J...>) {
                    return subdivide(
                        nodes[J]..., coordinate_system, subdivided_mesh, n_refinements);
                };

            // recursively subdivide the cell identified by these vertices
            _subdivide(
                nodes, coordinate_system, subdivided_mesh, n_refinements,
                std::make_index_sequence<cellT::order + 1> {});
        }

        // return the refined mesh
        return subdivided_mesh;
    }

}


#endif

// end of file
