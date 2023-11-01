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
        OrientedSimplex(OrientedSimplex &&) = delete;

        // delete assignment operator
        OrientedSimplex & operator=(const OrientedSimplex &) = delete;

        // delete move assignment operator
        OrientedSimplex & operator=(OrientedSimplex &&) = delete;

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

        // returns the array of subsimplices
        inline auto composition() const noexcept -> const simplex_composition_t<N> &
        {
            return _footprint->composition();
        }

        // append the vertices of this simplex to a collection of vertices
        template <class VERTEX_COLLECTION_T>
        inline auto vertices(VERTEX_COLLECTION_T & vertices) const -> void
        {
            return _footprint->vertices(vertices);
        }

        // return the array of vertices of this simplex
        inline auto vertices() const -> vertex_simplex_composition_t<N>
        requires(N > 0)
        {
            // fetch the vertices of this simplex
            auto vertices_collection = _footprint->vertices();

            // an array to store the composition of a N-simplex in terms of vertices
            vertex_simplex_composition_t<N> vertices;

            // assert that you found the correct number of vertices
            assert(std::size(vertices_collection) == std::size(vertices));

            // populate the array of vertices
            std::copy(vertices_collection.begin(), vertices_collection.end(), vertices.begin());

            // return the array of vertices
            return vertices;
        }

        // return the array of edge-directors of this simplex
        inline auto directors() const -> edge_simplex_directors_t<N>
        requires(N > 0)
        {
            // delegate question to the footprint
            return _footprint->directors();
        }

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
