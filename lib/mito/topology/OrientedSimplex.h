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
        constexpr OrientedSimplex(const simplex_ptr<D> & footprint, bool orientation) :
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

        // TOFIX: do we need both?
      public:
        // accessor for the footprint simplex
        const auto & simplex() const { return *_footprint.get(); }

        // accessor for the footprint shared pointer
        const auto & footprint() const { return _footprint; }

        // returns the orientation of this simplex
        // (true: oriented simplex is oriented as the footprint,
        //  false: oriented simplex is oriented opposite to the footprint)
        bool orientation() const { return _orientation; }

        // returns the array of subsimplices
        const auto & simplices() const { return _footprint.get()->simplices(); }

        // returns the set of vertices
        template <class VERTEX_COLLECTION_T>
        void vertices(VERTEX_COLLECTION_T & vertices) const
        {
            return _footprint.get()->vertices(vertices);
        }

        // returns whether the simplex passes the sanity check
        bool sanityCheck() const { return _footprint.get()->sanityCheck(); }

      public:
        // the number of barycentric coordinates
        static constexpr int parametricDim = D + 1;

      private:
        // the shared pointer to the footprint
        const simplex_ptr<D> _footprint;
        // the orientation
        bool _orientation;
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
        // accessor for the footprint simplex
        const auto & simplex() const { return *this; }

        // perform a sanity check
        bool sanityCheck() const
        {
            // a simplex of order 0 has only 1 vertex (this one!)
            return true;
        }
    };

    // overload operator<< for oriented simplices
    template <int D>
    std::ostream & operator<<(std::ostream & os, const OrientedSimplex<D> & s)
    {
        // print orientation
        os << "orientation: " << s.orientation() << std::endl;
        // print footprint
        os << "footprint: " << s.simplex() << std::endl;
        // all done
        return os;
    }

    // overload operator<< specialization for simplices with D = 0 (vertices)
    template <>
    std::ostream & operator<<(std::ostream & os, const OrientedSimplex<0> & s)
    {
        os << &s;
        return os;
    }

    auto tail(const oriented_simplex_ptr<1> & oriented_simplex)
    {
        if (oriented_simplex->orientation()) {
            return oriented_simplex->simplices()[0];
        } else {
            return oriented_simplex->simplices()[1];
        }
    }

    auto head(const oriented_simplex_ptr<1> & oriented_simplex)
    {
        if (oriented_simplex->orientation()) {
            return oriented_simplex->simplices()[1];
        } else {
            return oriented_simplex->simplices()[0];
        }
    }
}
#endif    // mito_topology_OrientedSimplex_h

// end of file
