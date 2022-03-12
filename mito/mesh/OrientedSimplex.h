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
      public:
        OrientedSimplex(const std::shared_ptr<simplex_t<D>> footprint, bool orientation) :
            _footprint(footprint), _orientation(orientation)
        {}

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
      
      public:
        const auto footprint() const {return _footprint;}
        bool orientation() const { return _orientation; }

      private:
        const std::shared_ptr<simplex_t<D>> _footprint;
        bool _orientation;
    };

#if 1
    template <>
    class OrientedSimplex<0> {
      public:
        OrientedSimplex(const std::shared_ptr<simplex_t<0>> footprint) : 
            _footprint(footprint) {}
        
        ~OrientedSimplex() {}

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
        const auto footprint() const { return _footprint; }

      private:
        const std::shared_ptr<simplex_t<0>> _footprint;
    };
#endif
}
#endif    // mito_mesh_OrientedSimplex_h

// end of file
