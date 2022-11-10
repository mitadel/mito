// code guard
#if !defined(mito_mesh_Boundary_h)
#define mito_mesh_Boundary_h


namespace mito::mesh {

    /**
     * @brief Returns an element set with all boundary elements of dimension I
     */
    // QUESTION: I don't like the asymmetry of elements returning a const reference and boundary
    //  elements returning an instance. Either:
    //  1) say that these methods will make copies of the elements for the client to use, or
    //  2) say that boundary_elements will create a new data structure at run time and return a
    //      (const) reference for the client to use.
    // TOFIX: this method should return an iterator
    template <int I, int D, template <int> class elementT>
    auto boundary(const mesh_t<D, elementT> & mesh, const topology_t & topology) -> auto
    requires(I<D && I> 0)
    {
        // instantiate an element collection
        element_set_t<elementT<I>> boundary_elements;

        // loop on the (I+1) dimensional elements
        for (const auto & element : mesh.template elements<I + 1>()) {
            for (const auto & subelement : element->composition()) {
                // if the element footprint has only one occurrence then it is on the boundary
                if (!topology.exists_flipped(subelement)) {
                    // add this (D-1) dimensional element to the set of boundary elements
                    boundary_elements.insert(subelement);
                }
            }
        }

        // return the boundary elements
        return boundary_elements;
    }
}


#endif    // mito_mesh_Mesh_h

// end of file
