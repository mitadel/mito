// code guard
#if !defined(mito_mesh_Boundary_h)
#define mito_mesh_Boundary_h


namespace mito::mesh {

    /**
     * @brief Returns a cell set with all boundary cells of dimension I
     */
    // QUESTION: I don't like the asymmetry of cells returning a const reference and boundary
    //  cells returning an instance. Either:
    //  1) say that these methods will make copies of the cells for the client to use, or
    //  2) say that boundary_cells will create a new data structure at run time and return a
    //      (const) reference for the client to use.
    // TOFIX: this method should return an iterator
    template <int I, int D, template <int> class cellT>
    auto boundary(const mesh_t<D, cellT> & mesh, const topology_t & topology) -> auto
    requires(I<D && I> 0)
    {
        // instantiate an cell collection
        element_set_t<cellT<I>> boundary_cells;

        // loop on the (I+1) dimensional cells
        for (const auto & cell : mesh.template cells<I + 1>()) {
            for (const auto & subcell : cell->composition()) {
                // if the cell footprint has only one occurrence then it is on the boundary
                if (!topology.exists_flipped(subcell)) {
                    // add this (D-1) dimensional cell to the set of boundary cells
                    boundary_cells.insert(subcell);
                }
            }
        }

        // return the boundary cells
        return boundary_cells;
    }
}


#endif    // mito_mesh_Mesh_h

// end of file
