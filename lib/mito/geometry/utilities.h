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

    // return the array of director vectors of the simplex and the coordinates of the point
    // where they stem from
    template <int N, int D, CoordinateType coordT>
    inline auto directors(
        const topology::simplex_t<N> & simplex, const geometry_t<D, coordT> & geometry)
        -> std::pair<coordinates_t<D, coordT>, edge_simplex_directors_t<N, D>>
    {
        // helper function to expand {nodes[1] - nodes[0]}, ..., {nodes[N-1] - nodes[0]}
        constexpr auto _directors = []<int... J>(
                                        const topology::simplex_t<N> & simplex,
                                        const geometry_t<D, coordT> & geometry,
                                        integer_sequence<J...>)
            -> std::pair<coordinates_t<D, coordT>, edge_simplex_directors_t<N, D>> {
            // get the simplex vertices
            auto vertices = simplex->vertices();

            // get the coordinates of the first node
            const auto & p0 = geometry.point(vertices[0])->coordinates();

            // compute the director vectors associated with each director edge
            auto directors = edge_simplex_directors_t<N, D> { (
                geometry.point(vertices[J + 1])->coordinates() - p0)... };

            // all done
            return { p0, directors };
        };

        return _directors(simplex, geometry, make_integer_sequence<N> {});
    }
}


#endif

// end of file
