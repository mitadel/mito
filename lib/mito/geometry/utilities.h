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
    auto barycenter(const cellT & cell, const coordinate_system_t<D, coordT> & coordinate_system)
        -> coordinates_t<D, coordT>
    {
        // the barycenter
        coordinates_t<D, coordT> result;

        // average the position of each vertex
        for (const auto & node : cell.nodes()) {
            result += coordinate_system.coordinates(node.point());
        }
        result /= cellT::n_vertices;

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
        const geometric_simplex_t<N, D> & simplex,
        const coordinate_system_t<D, coordT> & coordinate_system)
        -> std::pair<coordinates_t<D, coordT>, edge_simplex_directors_t<N, D>>
    {
        // helper function to expand {nodes[1] - nodes[0]}, ..., {nodes[N-1] - nodes[0]}
        constexpr auto _directors = []<int... J>(
                                        const geometric_simplex_t<N, D> & simplex,
                                        const coordinate_system_t<D, coordT> & coordinate_system,
                                        integer_sequence<J...>)
            -> std::pair<coordinates_t<D, coordT>, edge_simplex_directors_t<N, D>> {
            // get the simplex nodes
            auto nodes = simplex.nodes();

            // get the coordinates of the first node
            const auto & p0 = coordinate_system.coordinates(nodes[0].point());

            // compute the director vectors associated with each director edge
            auto directors = edge_simplex_directors_t<N, D>{ (
                coordinate_system.coordinates(nodes[J + 1].point()) - p0)... };

            // all done
            return { p0, directors };
        };

        return _directors(simplex, coordinate_system, make_integer_sequence<N>{});
    }

    // builds a geometric simplex based on a topological simplex {simplex} with the vertex-point
    // pairing as appears in {nodes}
    template <int D, int N, class nodesT>
    auto geometric_simplex(const topology::simplex_t<N> & simplex, const nodesT & nodes)
        -> geometric_simplex_t<N, D>
    {
        using vertex_type = topology::vertex_t;
        using node_type = node_t<D>;
        using geometric_simplex_type = geometric_simplex_t<N, D>;

        // helper function to expand {node_type(vertex, *std::find(vertex))} as many times as the
        // vertices of {simplex}
        auto _build_geometric_simplex =
            [&simplex, &nodes]<int... K>(integer_sequence<K...>) -> geometric_simplex_type {
            // get the vertices
            auto vertices = simplex->vertices();

            // helper function that returns a lambda function checking if a node corresponds to a
            // given vertex
            auto has_vertex = [](const vertex_type & vertex) {
                auto lambda = [&vertex](const node_type & node) {
                    return node.vertex() == vertex;
                };
                return lambda;
            };

            // return the geometric simplex
            return geometric_simplex_type({ node_type(
                vertices[K],
                std::find_if(nodes.begin(), nodes.end(), has_vertex(vertices[K]))->point())... });
        };

        // build a geometric simplex based on {simplex} with the vertex-point pair as appears in
        // {nodes}
        return _build_geometric_simplex(make_integer_sequence<N + 1>());
    }

    template <int D>
    auto flip_diagonal(
        const std::pair<geometric_simplex_t<2, D>, geometric_simplex_t<2, D>> & simplex_pair)
        -> std::pair<geometric_simplex_t<2, D>, geometric_simplex_t<2, D>>
    {
        // get the two geometric simplices
        auto simplex_0 = simplex_pair.first;
        auto simplex_1 = simplex_pair.second;

        // flip the topological simplices
        auto flipped_simplex_pair =
            mito::topology::flip_diagonal({ simplex_0.simplex(), simplex_1.simplex() });

        // concatenate in {nodes} the nodes of the two simplices
        using node_type = node_t<D>;
        std::vector<node_type> nodes;
        nodes.insert(nodes.begin(), simplex_0.nodes().begin(), simplex_0.nodes().end());
        nodes.insert(nodes.begin(), simplex_1.nodes().begin(), simplex_1.nodes().end());

        // build the new geometric simplices and return them
        return { geometric_simplex<D>(flipped_simplex_pair.first, nodes),
                 geometric_simplex<D>(flipped_simplex_pair.second, nodes) };
    }
}


#endif

// end of file
