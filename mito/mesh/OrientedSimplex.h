// code guard
#if !defined(mito_mesh_OrientedSimplex_h)
#define mito_mesh_OrientedSimplex_h

/*
 * This class represents a class of equivalence for an OrientedSimplex of dimension D.
 *
 * Each instance of OrientedSimplex consists of an underlying (nonoriented) Simplex and an
 * orientation, which can be positive or negative.
 * OrientedSimplex<D> is represented recursively as a collection of D+1 OrientedSimplex<D-1>. An
 * instance of Simplex<D> represents therefore (D+1)!/2 simplices, depending on the (D+1)!/2
 * possible orderings of the subsimplices with positive (negative) sign.
 *
 * When the orientation is positive, the representative in this class of equivalence is chosen by
 * taking the first subsimplex of the underlying (nonoriented) Simplex with its natural orientation.
 * When the orientation is negative, the representative in this class of equivalence is chosen by
 * taking the first subsimplex of the underlying (nonoriented) Simplex against its natural
 * orientation.
 */

namespace mito::mesh {

    template <int D>
    class OrientedSimplex {
      private:
        // constructor with an existing shared pointer as footprint
        OrientedSimplex(const std::shared_ptr<const simplex_t<D>> & footprint, bool orientation) :
            _footprint(footprint),
            _orientation(orientation)
        {}

        // constructor with a raw pointer as footprint (builds shared pointer around raw pointer)
        OrientedSimplex(const simplex_t<D> * footprint, bool orientation) :
            _footprint(footprint),
            _orientation(orientation)
        {}

      public:
        ~OrientedSimplex() {}

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
        const auto & footprint() const { return _footprint; }

      public:
        const auto & simplex() const { return *_footprint.get(); }
        // returns whether there exists the flipped simplex in the factory
        bool exists_flipped() const 
        {
            // assert the footprint cannot be used by more than two oriented simplices
            assert(_footprint.use_count() == 1 || _footprint.use_count() == 2);
            // return true if the footprint is in used by two oriented simpilces 
            return _footprint.use_count() == 2 ? true : false;
        }
        // returns the number of simplices riding on this oriented simplex
        int incidence() const 
        {
            return OrientedSimplexFactory<D>::find(*this).use_count() - 1;
        }
        bool orientation() const { return _orientation; }
        const auto & simplices() const { return _footprint.get()->simplices(); }
        void vertices(vertex_set_t & vertices) const { return _footprint.get()->vertices(vertices); }
        bool sanityCheck() const { return _footprint.get()->sanityCheck(); }

        auto flip() const
        {
            return OrientedSimplexFactory<D>::orientedSimplex(*_footprint.get(), !_orientation);
        }

        template <int I>
        void getSimplices(simplex_set_t<oriented_simplex_t<I>> & sub_simplices) const requires(
            I < D - 1 && I != 0)
        {
            for (const auto & simplex : simplices()) {
                simplex->simplices<I - 1>(sub_simplices);
            }
            return;
        }

        void getSimplices(vertex_set_t & sub_simplices) const
        {
            return vertices(sub_simplices);
        }
        template <int I>
        void getSimplices(simplex_set_t<oriented_simplex_t<I>> & sub_simplices) const
            requires(I == D)
        {
            sub_simplices.insert(OrientedSimplexFactory<D>::find(*this));
            return;
        }
        template <int I>
        void getSimplices(simplex_set_t<oriented_simplex_t<I>> & sub_simplices) const requires(
            I == D - 1 && I != 0)
        {
            for (const auto & simplex : simplices()) {
                auto & sub_simplex = oriented_simplex(simplex.simplices());
                sub_simplices.insert(sub_simplex);
            }
            return;
        }

      public:
        static constexpr int parametricDim = D + 1;
        static constexpr int nVertices = D;

      private:
        const std::shared_ptr<const simplex_t<D>> _footprint;
        bool _orientation;

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
