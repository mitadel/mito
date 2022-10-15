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
    class OrientedSimplex {

        // private constructors: only the OrientedSimplexFactory has the right to instantiate
        // oriented simplices
      private:
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
        const auto & footprint() const { return _footprint; }

        // returns the orientation of this simplex
        // (true: oriented simplex is oriented as the footprint,
        //  false: oriented simplex is oriented opposite to the footprint)
        bool orientation() const { return _orientation; }

        // returns the array of subsimplices
        const auto & composition() const { return _footprint->composition(); }

        // returns the id of this (oriented) simplex
        oriented_simplex_id_t<D> id() const
        {
            // the id is the (immutable) address of this object
            return reinterpret_cast<unoriented_simplex_id_t<D>>(this);
        }

        // returns the id of this (oriented) simplex
        oriented_simplex_id_t<D> simplex_id() const { return id(); }

        // returns the (unoriented) footprint id
        // (the footprint id is the (immutable) address of the unoriented footprint)
        unoriented_simplex_id_t<D> footprint_id() const { return _footprint->id(); }

        // returns the set of vertices
        template <class VERTEX_COLLECTION_T>
        void vertices(VERTEX_COLLECTION_T & vertices) const
        {
            return _footprint->vertices(vertices);
        }

        // returns whether the simplex passes the sanity check
        inline auto sanityCheck() const -> bool { return _footprint->sanityCheck(); }

      private:
        // the shared pointer to the footprint
        const unoriented_simplex_ptr<D> _footprint;
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
    class OrientedSimplex<0> {
      public:
        // default constructor
        constexpr OrientedSimplex() {}

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
        inline auto footprint_id() const -> unoriented_simplex_id_t<0>
        {
            // the id is the (immutable) address of this object
            return reinterpret_cast<unoriented_simplex_id_t<0>>(this);
        }

        // perform a sanity check
        inline auto sanityCheck() const -> bool
        {
            // a simplex of order 0 has only 1 vertex (this one!)
            return true;
        }
    };

    // overload operator<< for oriented simplices
    template <int D>
    std::ostream & operator<<(std::ostream & os, const simplex_t<D> & s)
    {
        // print orientation
        os << "orientation: " << s->orientation() << std::endl;
        // print footprint
        os << "footprint: " << s->footprint() << std::endl;
        // all done
        return os;
    }

    // overload operator<< specialization for simplices with D = 0 (vertices)
    template <>
    std::ostream & operator<<(std::ostream & os, const simplex_t<0> & s)
    {
        os << s->footprint_id();
        return os;
    }

    auto tail(const simplex_t<1> & oriented_simplex)
    {
        if (oriented_simplex->orientation()) {
            return oriented_simplex->composition()[0];
        } else {
            return oriented_simplex->composition()[1];
        }
    }

    auto head(const simplex_t<1> & oriented_simplex)
    {
        if (oriented_simplex->orientation()) {
            return oriented_simplex->composition()[1];
        } else {
            return oriented_simplex->composition()[0];
        }
    }
}
#endif    // mito_topology_OrientedSimplex_h

// end of file
