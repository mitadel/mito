// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_utilities_h)
#define mito_geometry_utilities_h


namespace mito::geometry {

    // compute the barycenter of a cell, given a geometry
    template <class cellT, int D, CoordinateType coordT>
    auto barycenter(
        const cellT & cell, const geometry_t<D> & geometry,
        const coordinate_system_t<D, coordT> & coordinate_system) -> coordinates_t<D, coordT>
    {
        // the barycenter
        coordinates_t<D, coordT> result;

        // average the position of each vertex
        for (const auto & vertex : cell->vertices()) {
            result += coordinate_system.coordinates(geometry.point(vertex));
        }
        result /= topology::n_vertices<cellT>();

        // all done
        return result;
    }

    // compute the distance between two points
    template <int D, CoordinateType coordT>
    constexpr auto distance(
        const point_t<D> & pointA, const point_t<D> & pointB,
        const coordinate_system_t<D, coordT> & coordinate_system) -> real
    {
        // return the distance between the two points
        return distance(
            coordinate_system.coordinates(pointA), coordinate_system.coordinates(pointB));
    }

    // return the array of director vectors of the simplex and the coordinates of the point
    // where they stem from
    template <int N, int D, CoordinateType coordT>
    inline auto directors(
        const topology::simplex_t<N> & simplex, const geometry_t<D> & geometry,
        const coordinate_system_t<D, coordT> & coordinate_system)
        -> std::pair<coordinates_t<D, coordT>, edge_simplex_directors_t<N, D>>
    {
        // helper function to expand {nodes[1] - nodes[0]}, ..., {nodes[N-1] - nodes[0]}
        constexpr auto _directors =
            []<int... J>(
                const topology::simplex_t<N> & simplex, const geometry_t<D> & geometry,
                const coordinate_system_t<D, coordT> & coordinate_system, integer_sequence<J...>)
            -> std::pair<coordinates_t<D, coordT>, edge_simplex_directors_t<N, D>> {
            // get the simplex vertices
            auto vertices = simplex->vertices();

            // get the coordinates of the first node
            const auto & p0 = coordinate_system.coordinates(geometry.point(vertices[0]));

            // compute the director vectors associated with each director edge
            auto directors = edge_simplex_directors_t<N, D>{ (
                coordinate_system.coordinates(geometry.point(vertices[J + 1])) - p0)... };

            // all done
            return { p0, directors };
        };

        return _directors(simplex, geometry, coordinate_system, make_integer_sequence<N>{});
    }
}


#endif

// end of file
