// code guard
#if !defined(mito_geometry_utilities_h)
#define mito_geometry_utilities_h


namespace mito::geometry {

    // compute the barycenter of a cell, given a geometry
    template <class cellT, int D, CoordinateType coordT>
    auto barycenter(const cellT & cell, const geometry_t<D, coordT> & geometry)
        -> coordinates_t<D, coordT>
    {
        // the barycenter
        coordinates_t<D, coordT> result;

        // average the position of each vertex
        for (const auto & vertex : cell->vertices()) {
            result += geometry.point(vertex)->coordinates();
        }
        result /= topology::n_vertices<cellT>();

        // all done
        return result;
    }
}


#endif

// end of file
