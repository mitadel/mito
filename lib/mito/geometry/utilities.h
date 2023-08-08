// code guard
#if !defined(mito_geometry_utilities_h)
#define mito_geometry_utilities_h


namespace mito::geometry {

    // compute the barycenter of a cell, given a geometry
    template <class cellT, int D>
    auto barycenter(const cellT & cell, const geometry_t<D> & geometry) -> vector_t<D>
    {
        // the barycenter
        vector_t<D> result;

        // average the position of each vertex
        for (const auto & vertex : cell->vertices()) {
            result += geometry.point(vertex)->coordinates();
        }
        result /= cellT::resource_t::n_vertices;

        // all done
        return result;
    }
}


#endif

// end of file
