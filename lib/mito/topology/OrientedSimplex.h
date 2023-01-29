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
    class OrientedSimplex : public mito::utilities::Shareable {
      public:
        // typedef for order of simplex
        static constexpr int order = D;

        // typedef for the cell family type (simplicial)
        template <int N>
        using cell_family_t = simplex_t<N>;

        // private constructors: only the OrientedSimplexFactory has the right to instantiate
        // oriented simplices
      public:    // TOFIX: should be private
        // constructor with an existing shared pointer as footprint
        constexpr OrientedSimplex(const unoriented_simplex_ptr<D> & footprint, bool orientation) :
            _footprint(footprint),
            _orientation(orientation)
        {}

      public:
        // destructor
        constexpr ~OrientedSimplex() {}

      private:
        // delete default constructor
        OrientedSimplex() = delete;

        // delete copy constructor
        OrientedSimplex(const OrientedSimplex &) = delete;

        // delete move constructor
        OrientedSimplex(const OrientedSimplex &&) = delete;

        // delete assignment operator
        const OrientedSimplex & operator=(const OrientedSimplex &) = delete;

        // delete move assignment operator
        const OrientedSimplex & operator=(const OrientedSimplex &&) = delete;

      public:
        // accessor for the unoriented footprint
        inline auto footprint() const -> const unoriented_simplex_ptr<D> & { return _footprint; }

        // returns the orientation of this simplex
        // (true: oriented simplex is oriented as the footprint,
        //  false: oriented simplex is oriented opposite to the footprint)
        inline bool orientation() const { return _orientation; }

        // returns the array of subsimplices
        inline auto composition() const -> const auto & { return _footprint->composition(); }

        // returns the id of this (oriented) simplex
        inline auto id() const -> oriented_simplex_id_t
        {
            // the id is the (immutable) address of this object
            return reinterpret_cast<unoriented_simplex_id_t>(this);
        }

        // returns the id of this (oriented) simplex
        inline auto simplex_id() const -> oriented_simplex_id_t { return id(); }

        // returns the (unoriented) footprint id
        // (the footprint id is the (immutable) address of the unoriented footprint)
        inline auto footprint_id() const -> unoriented_simplex_id_t { return _footprint->id(); }

        // returns the set of vertices
        template <class VERTEX_COLLECTION_T>
        inline auto vertices(VERTEX_COLLECTION_T & vertices) const -> void
        {
            return _footprint->vertices(vertices);
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
        unoriented_simplex_ptr<D> _footprint;    // TOFIX: should this be {const}?
        // the orientation
        const bool _orientation;
        // private friendship with the factory of oriented simplices
        friend class OrientedSimplexFactory<D>;
    };

    /*
     * This class collapses OrientedSimplex<D> for D = 0.
     *
     * A simplex of order 0, like a vertex, is an empty object.
     */

    template <>
    class OrientedSimplex<0> : public mito::utilities::Shareable {
      public:    // TOFIX: should be private
        // default constructor
        constexpr OrientedSimplex() {}

      public:
        // empty destructor
        constexpr ~OrientedSimplex() {}

      private:
        // delete copy constructor
        OrientedSimplex(const OrientedSimplex &) = delete;

        // delete move constructor
        OrientedSimplex(const OrientedSimplex &&) = delete;

        // delete assignment operator
        const OrientedSimplex & operator=(const OrientedSimplex &) = delete;

        // delete move assignment operator
        const OrientedSimplex & operator=(const OrientedSimplex &&) = delete;

      public:
        // returns the (unoriented) footprint id
        // Note: the footprint of a vertex is the vertex itself
        inline auto footprint_id() const -> unoriented_simplex_id_t
        {
            // the id is the (immutable) address of this object
            return reinterpret_cast<unoriented_simplex_id_t>(this);
        }

        // perform a sanity check
        inline auto sanityCheck() const -> bool
        {
            // a simplex of order 0 has only 1 vertex (this one!)
            return true;
        }

      private:
        inline auto _erase() -> void
        {
            // all done
            return;
        }

      private:
        // friendship with the factory of oriented simplices
        friend class OrientedSimplexFactory<0>;
    };
}
#endif    // mito_topology_OrientedSimplex_h

// end of file
