// code guard
#if !defined(mito_mesh_OrientedSimplex_h)
#define mito_mesh_OrientedSimplex_h

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

namespace mito::mesh {

    template <int D>
    class OrientedSimplex {

        // private constructors: only the OrientedSimplexFactory has the right to instantiate
        // oriented simplices
      private:
        // constructor with an existing shared pointer as footprint
        constexpr OrientedSimplex(
            const std::shared_ptr<const simplex_t<D>> & footprint, bool orientation) :
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

      private:
        // accessor for the footprint shared pointer
        const auto & footprint() const { return _footprint; }

      public:
        // accessor for the footprint simplex
        const auto & simplex() const { return *_footprint.get(); }

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

        // TOFIX: these should be helper methods in mesh, not sure it makes sense to have them as 
        // part of the OrientedSimplex interface
        // get the set of subsimplices of order I
        template <int I>
        void getSimplices(simplex_set_t<oriented_simplex_t<I>> & sub_simplices) const
            requires(I < D - 1 && I != 0)
        {
            for (const auto & simplex : simplices()) {
                simplex->simplices<I - 1>(sub_simplices);
            }
            return;
        }
        void getSimplices(vertex_set_t & sub_simplices) const { return vertices(sub_simplices); }
        template <int I>
        void getSimplices(simplex_set_t<oriented_simplex_t<I>> & sub_simplices) const
            requires(I == D)
        {
            sub_simplices.insert(OrientedSimplexFactory<D>::find(*this));
            return;
        }
        template <int I>
        void getSimplices(simplex_set_t<oriented_simplex_t<I>> & sub_simplices) const
            requires(I == D - 1 && I != 0)
        {
            for (const auto & simplex : simplices()) {
                auto & sub_simplex = oriented_simplex(simplex.simplices());
                sub_simplices.insert(sub_simplex);
            }
            return;
        }

      public :
          // the number of barycentric coordinates
          static constexpr int parametricDim = D + 1;

      private:
        // the shared pointer to the footprint
        const std::shared_ptr<const simplex_t<D>> _footprint;
        // the orientation
        bool _orientation;
        // private frienship with the factory of oriented simplices
        friend class OrientedSimplexFactory<D>;
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
}
#endif    // mito_mesh_OrientedSimplex_h

// end of file
