// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#pragma once

/*
 * This class represents an oriented simplex of order N in D-dimensional space.
 *
 * Each instance of {GeometricSimplex} consists of an underlying {OrientedSimplex} and a
 * vertex-point mapping.
 *
 */

namespace mito::geometry {

    template <int N, int D>
    requires((N >= 0) && (N <= D) && (D > 0))
    class GeometricSimplex : public utilities::Invalidatable {

      public:
        // spatial dimension
        static constexpr int dim = D;

        // order of simplex
        static constexpr int order = N;

        // the type of the vertices
        using vertex_type = topology::vertex_t;

        // the type of the topological simplex
        using simplex_type = topology::oriented_simplex_t<N>;

        // the type of the unoriented simplex
        using unoriented_simplex_type = topology::unoriented_simplex_t<N>;

        // number of vertices of simplex
        static constexpr int n_vertices = topology::n_vertices<simplex_type>();

        // the node type
        using node_type = node_t<D>;

        // a collection of nodes
        using nodes_type = std::array<node_type, N + 1>;

        // typedef for the family type (geometric simplicial)
        template <int NN, int DD>
        using cell_family_type = geometric_simplex_t<NN, DD>;

        // typedef for the topological family type (simplicial)
        template <int I>
        using cell_topological_family_type = typename topology::cell_family<simplex_type, I>;

        // type of a point in barycentric coordinates
        using barycentric_coordinates_type =
            typename simplex_type::resource_type::barycentric_coordinates_type;

      private:
        constexpr auto _sanity_check() const -> bool
        {
            // helper function to check that the order of vertices in {_simplex} matches the order
            // of nodes in {_nodes}
            auto _check_vertices = [this]<int... J>(tensor::integer_sequence<J...>) -> bool {
                return (
                    math::permutation_sign(_simplex->vertices(), { _nodes[J]->vertex()... }) == +1);
            };

            return _check_vertices(tensor::make_integer_sequence<N + 1>{});
        }

        template <int... J>
        constexpr auto _create_simplex(tensor::integer_sequence<J...>) const -> simplex_type
        {
            // fetch the topology
            auto & topology = topology::topology();

            // instantiate a simplex with the vertices prescribed by {_nodes}
            return topology.simplex<N>({ _nodes[J]->vertex()... });
        }

      public:
        // constructor with an existing oriented simplex and a collection of nodes
        constexpr GeometricSimplex(const simplex_type & simplex, const nodes_type & nodes) :
            Invalidatable(),
            _nodes(nodes),
            _simplex(simplex)
        {
            // check that the vertices in {nodes} match the vertices of the {simplex} within a
            // positive permutation
            assert(_sanity_check());
        }

        // QUESTION: do we need this method?
        // constructor with an existing oriented simplex and a collection of nodes
        constexpr GeometricSimplex(const nodes_type & nodes) :
            Invalidatable(),
            _nodes(nodes),
            _simplex(_create_simplex(tensor::make_integer_sequence<n_vertices>{}))
        {}

        // move constructor
        constexpr GeometricSimplex(GeometricSimplex &&) noexcept = default;

        // copy constructor
        constexpr GeometricSimplex(const GeometricSimplex &) = default;

        // destructor
        constexpr ~GeometricSimplex() = default;

        // default assignment operator
        constexpr GeometricSimplex & operator=(const GeometricSimplex &) = default;

        // default move assignment operator
        constexpr GeometricSimplex & operator=(GeometricSimplex &&) noexcept = default;

      private:
        // delete default constructor
        constexpr GeometricSimplex() = delete;

      public:
        // TOFIX: possibly change {simplex} into {footprint} to remove the baggage of simplicial
        // elements
        // accessor for the underlying oriented simplex
        constexpr auto simplex() const noexcept -> const simplex_type & { return _simplex; }

        // accessor for the underlying oriented simplex
        constexpr auto simplex() noexcept -> simplex_type & { return _simplex; }

        // return the composition of this simplex in terms of its vertices
        constexpr auto nodes() const -> const nodes_type & { return _nodes; }

        template <coordinate_system_c coordinateSystemT>
        constexpr auto parametrization(
            const barycentric_coordinates_type & point,
            const coordinateSystemT & coordinate_system) const
            -> coordinateSystemT::coordinates_type
        {
            // get the coordinates of the first node
            auto coord_0 = coordinate_system.coordinates(_nodes[0]->point());

            // the vector going from {coord_0} to the position of the parametric point (initialize
            // with the zero vector)
            auto result = coord_0 - coord_0;

            // loop on the element nodes
            int v = 0;
            for (const auto & node : _nodes) {
                result += (coordinate_system.coordinates(node->point()) - coord_0) * point[v];
                ++v;
            }

            // return the coordinates of the parametric point
            return coord_0 + result;
        }

      private:
        // the simplex nodes
        nodes_type _nodes;
        // the shared pointer to the footprint
        simplex_type _simplex;
    };

    template <int D>
    requires(D > 0)
    class GeometricSimplex<0, D> : public utilities::Invalidatable {

      public:
        // spatial dimension
        static constexpr int dim = D;

        // order of simplex
        static constexpr int order = 0;

        // the type of the vertices
        using vertex_type = topology::vertex_t;

        // the point type
        using point_type = point_t<D>;

      public:
        // get the coordinates of the point
        GeometricSimplex(const vertex_type & vertex, const point_type & point) :
            _vertex(vertex),
            _point(point)
        {}

        // move constructor
        constexpr GeometricSimplex(GeometricSimplex &&) noexcept = default;

        // copy constructor
        constexpr GeometricSimplex(const GeometricSimplex &) = default;

        // destructor
        constexpr ~GeometricSimplex() = default;

        // default assignment operator
        constexpr GeometricSimplex & operator=(const GeometricSimplex &) = default;

        // default move assignment operator
        constexpr GeometricSimplex & operator=(GeometricSimplex &&) noexcept = default;

      private:
        // delete default constructor
        constexpr GeometricSimplex() = delete;

      public:
        // accessor for the underlying vertex
        constexpr auto vertex() const noexcept -> const vertex_type & { return _vertex; }

        // accessor for the underlying point
        constexpr auto point() const noexcept -> const point_type & { return _point; }

      private:
        // the vertex that this node is attached to
        vertex_type _vertex;
        // the point that this node is attached to
        point_type _point;
    };

}


// end of file
