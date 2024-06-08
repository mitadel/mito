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

      private:
        constexpr auto _sanity_check() const -> bool
        {
            // helper function to check that the order of vertices in {_simplex} matches the order
            // of nodes in {_nodes}
            auto _check_vertices = [this]<int... J>(integer_sequence<J...>) -> bool {
                return (
                    math::permutation_sign(_simplex->vertices(), { _nodes[J]->vertex()... }) == +1);
            };

            return _check_vertices(make_integer_sequence<N + 1>{});
        }

        template <int... J>
        constexpr auto _create_simplex(integer_sequence<J...>) const -> simplex_type
        {
            // fetch the topology
            auto & topology = topology::topology();

            // instantiate a simplex with the vertices prescribed by {_nodes}
            return topology.simplex<N>({ _nodes[J]->vertex()... });
        }

      public:
        // constructor with an existing oriented simplex and a collection of nodes
        constexpr GeometricSimplex(const simplex_type & simplex, const nodes_type & nodes) :
            _nodes(nodes),
            _simplex(simplex)
        {
            // check that the vertices in {nodes} match the vertices of the {simplex} within a
            // positive permutation
            assert(_sanity_check());
        }

        // constructor with an existing oriented simplex and a collection of nodes
        constexpr GeometricSimplex(const nodes_type & nodes) :
            _nodes(nodes),
            _simplex(_create_simplex(make_integer_sequence<n_vertices>{}))
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

      private:
        // the simplex nodes
        nodes_type _nodes;
        // the shared pointer to the footprint
        simplex_type _simplex;
    };

    template <int D>
    requires(D > 0)
    class GeometricSimplex<0, D> : public utilities::Invalidatable, public utilities::Shareable {

      public:
        // spatial dimension
        static constexpr int dim = D;

        // order of simplex
        static constexpr int order = 0;

        // the type of the vertices
        using vertex_type = topology::vertex_t;

        // the type of the topological simplex
        using simplex_type = topology::oriented_simplex_t<0>;

        // number of vertices of simplex
        static constexpr int n_vertices = 0;

        // the node type
        using node_type = node_t<D>;

        // a collection of nodes
        using nodes_type = std::array<node_type, order + 1>;

        // the point type
        using point_type = point_t<D>;

        // typedef for the topological family type (simplicial)
        template <int I>
        using cell_topological_family_type = typename topology::cell_family<simplex_type, I>;

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
        auto vertex() const noexcept -> const vertex_type & { return _vertex; }

        // accessor for the underlying point
        auto point() const noexcept -> const point_type & { return _point; }

      private:
        // the vertex that this node is attached to
        vertex_type _vertex;
        // the point that this node is attached to
        point_type _point;
    };

    // operator== for nodes
    template <int D>
    constexpr auto operator==(
        const GeometricSimplex<0, D> & node_a, const GeometricSimplex<0, D> & node_b) -> bool
    {
        // two nodes are the same if they have the same id
        return node_a.id() == node_b.id();
    }
}


// end of file
