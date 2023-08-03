// code guard
#if !defined(mito_topology_OrientedSimplex_h)
#define mito_topology_OrientedSimplex_h

/*
 * This class represents an OrientedSimplex of order D.
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

    template <int D>
    class OrientedSimplex : public utilities::Shareable {
      public:
        // order of simplex
        static constexpr int order = D;

        // number of vertices of simplex
        static constexpr int n_vertices = order + 1;

        // typedef for the cell family type (simplicial)
        template <int N>
        using cell_family_t = simplex_t<N>;

        // private constructors: only the OrientedSimplexFactory has the right to instantiate
        // oriented simplices
      private:
        // constructor with an existing shared pointer as footprint
        constexpr OrientedSimplex(const unoriented_simplex_t<D> & footprint, bool orientation) :
            _footprint(footprint),
            _orientation(orientation)
        {}

      public:
        // destructor
        constexpr ~OrientedSimplex() override {}

      private:
        // delete default constructor
        OrientedSimplex() = delete;

        // delete copy constructor
        OrientedSimplex(const OrientedSimplex &) = delete;

        // delete move constructor
        OrientedSimplex(OrientedSimplex &&) = delete;

        // delete assignment operator
        OrientedSimplex & operator=(const OrientedSimplex &) = delete;

        // delete move assignment operator
        OrientedSimplex & operator=(OrientedSimplex &&) = delete;

      public:
        // accessor for the unoriented footprint
        inline auto footprint() const noexcept -> const unoriented_simplex_t<D> &
        {
            return _footprint;
        }

        // returns the orientation of this simplex
        // (true: oriented simplex is oriented as the footprint,
        //  false: oriented simplex is oriented opposite to the footprint)
        inline auto orientation() const noexcept -> bool { return _orientation; }

        // returns the array of subsimplices
        inline auto composition() const noexcept -> const simplex_composition_t<D> &
        {
            return _footprint->composition();
        }

        // append the vertices of this simplex to a collection of vertices
        template <class VERTEX_COLLECTION_T>
        inline auto vertices(VERTEX_COLLECTION_T & vertices) const -> void
        {
            return _footprint->vertices(vertices);
        }

        // append the edges of this simplex to a collection of edges
        template <class EDGES_COLLECTION_T>
        inline auto edges(EDGES_COLLECTION_T & edges) const -> void
        {
            return _footprint->edges(edges);
        }

        // returns whether the simplex passes the sanity check
        inline auto sanityCheck() const -> bool { return _footprint->sanityCheck(); }

      private:
        inline auto _erase() -> void
        {
            // reset the footprint shared pointer
            _footprint.reset();

            // all done
            return;
        }

      private:
        // the shared pointer to the footprint
        unoriented_simplex_t<D> _footprint;    // TOFIX: should this be {const}?
        // the orientation
        const bool _orientation;
        // private friendship with the factory of oriented simplices
        friend class OrientedSimplexFactory<D>;
    };
}
#endif    // mito_topology_OrientedSimplex_h

// end of file
