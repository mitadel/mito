// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_topology_OrientedSimplex_h)
#define mito_topology_OrientedSimplex_h

/*
 * This class represents an OrientedSimplex of order N.
 *
 * Each instance of OrientedSimplex consists of an underlying Simplex (the footprint) and an
 * orientation, which can be either zero or one. When the orientation is one, the OrientedSimplex
 * rides on the Simplex with the natural simplex orientation. When the orientation is zero, the
 * OrientedSimplex rides on the Simplex in opposite orientation.
 *
 * Two opposite instances of OrientedSimplex will share ownership of the underlying footprint via
 * a shared pointer. In case the two OrientedSimplex objects were to be deleted, the footprint will
 * also be deleted.
 */

namespace mito::topology {

    template <int N>
    class OrientedSimplex : public utilities::Shareable {
      public:
        // order of simplex
        static constexpr int order = N;

        // number of vertices of simplex
        static constexpr int n_vertices = order + 1;

        // typedef for the cell family type (simplicial)
        template <int I>
        using cell_family_type = simplex_t<I>;

        // private constructors: only the OrientedSimplexFactory has the right to instantiate
        // oriented simplices
      private:
        // constructor with an existing shared pointer as footprint
        constexpr OrientedSimplex(
            const unoriented_simplex_t<N> & footprint, orientation_t orientation) :
            _footprint(footprint),
            _orientation(orientation)
        {}

        // destructor
        constexpr ~OrientedSimplex() override {}

      private:
        // delete default constructor
        OrientedSimplex() = delete;

        // delete copy constructor
        OrientedSimplex(const OrientedSimplex &) = delete;

        // delete move constructor
        OrientedSimplex(OrientedSimplex &&) noexcept = delete;

        // delete assignment operator
        OrientedSimplex & operator=(const OrientedSimplex &) = delete;

        // delete move assignment operator
        OrientedSimplex & operator=(OrientedSimplex &&) noexcept = delete;

      public:
        // accessor for the unoriented footprint
        inline auto footprint() const noexcept -> const unoriented_simplex_t<N> &
        {
            return _footprint;
        }

        // returns the orientation of this simplex
        // (true: oriented simplex is oriented as the footprint,
        //  false: oriented simplex is oriented opposite to the footprint)
        inline auto orientation() const noexcept -> orientation_t { return _orientation; }

        // returns the ordered array of subsimplices
        inline auto composition() const -> simplex_composition_t<N>
        {
            // get a tentative composition from the composition of the footprint
            auto composition = _footprint->composition();
            // if the orientation of this simplex is opposite to that of the footprint
            if (_orientation == -1) {
                // perform an odd permutation on {composition}
                std::swap(composition[0], composition[1]);
            }
            // return the simplex {composition}
            return composition;
        }

        // append the vertices of this simplex to a collection of vertices
        inline auto vertices(VertexInsertable auto & vertices) const -> void
        {
            // get the vertices from the footprint
            return _footprint->vertices(vertices);
        }

        // append the vertices of this simplex to a collection of vertices
        inline auto vertices(VertexPushBackable auto & vertices) const -> void
        requires(N > 0)
        {
            // get a tentative collection of vertices from the footprint
            _footprint->vertices(vertices);

            // if the orientation of this simplex is opposite to that of the footprint
            if (_orientation == -1) {
                // perform an odd permutation
                std::swap(vertices[0], vertices[1]);
            }

            // all done
            return;
        }

        // append the vertices of this simplex to a collection of vertices
        inline auto vertices(VertexPushBackable auto & vertices) const -> void
        requires(N == 0)
        {
            // search for the footprint of this oriented vertex in the vertex collection {vertices}
            auto found = std::ranges::find(vertices, _footprint);

            // if we have not collected it yet
            if (found == std::end(vertices)) {
                // add it to the collection
                vertices.push_back(_footprint);
            }

            // all done
            return;
        }

        // return the composition of this simplex in terms of its vertices
        inline auto vertices() const -> vertex_simplex_composition_t<N>
        requires(N > 0)
        {
            // array to store the composition of this N-simplex in terms of vertices
            vertex_simplex_composition_t<N> vertices;

            // instantiate a vector to collect the vertices of this simplex
            std::vector<vertex_t> vertices_subsimplices;
            vertices_subsimplices.reserve(std::size(vertices));

            // collect the vertices of this simplex recursively
            this->vertices(vertices_subsimplices);

            // assert that you found the correct number of vertices
            assert(std::size(vertices_subsimplices) == std::size(vertices));

            // populate the array of vertices
            std::ranges::copy(vertices_subsimplices, std::begin(vertices));

            // return the array of vertices
            return vertices;
        }

        // TOFIX
        // append the edges of this simplex to a collection of edges
        template <class EDGES_COLLECTION_T>
        inline auto edges(EDGES_COLLECTION_T & edges) const -> void
        {
            return _footprint->edges(edges);
        }

        // TOFIX: remove methods sanity check, a simplex does not need it
        // returns whether the simplex passes the sanity check
        inline auto sanityCheck() const -> bool { return _footprint->sanityCheck(); }

      private:
        // the shared pointer to the footprint
        const unoriented_simplex_t<N> _footprint;
        // the orientation
        const orientation_t _orientation;
        // private friendship with the repository of oriented simplices
        friend class utilities::Repository<oriented_simplex_t<N>>;
    };
}
#endif    // mito_topology_OrientedSimplex_h

// end of file
