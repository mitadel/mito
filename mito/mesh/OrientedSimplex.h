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
        OrientedSimplex(const std::shared_ptr<simplex_t<D>> & footprint, bool orientation) :
            _footprint(footprint),
            _orientation(orientation)
        {}

        // constructor with a raw pointer as footprint (builds shared pointer around raw pointer)
        OrientedSimplex(simplex_t<D> * footprint, bool orientation) :
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

      public:
        const auto & footprint() const { return _footprint; }
        bool orientation() const { return _orientation; }
        const auto & simplices() const { return _footprint.get()->simplices(); }
        void vertices(std::set<const vertex_t *> & vertices) const
        {
            return _footprint.get()->vertices(vertices);
        }
        bool sanityCheck() const { return _footprint.get()->sanityCheck(); }

      public:
        static constexpr int parametricDim = D + 1;
        static constexpr int nVertices = D;

      private:
        const std::shared_ptr<simplex_t<D>> _footprint;
        bool _orientation;

        friend class mito::mesh::OrientedSimplexFactory<D>;
    };

    template <int D>
    class OrientedSimplexComposition : public std::array<oriented_simplex_t<D - 1> *, D + 1> {
      public:
        // cast to simplex_composition_t<D>
        constexpr operator simplex_composition_t<D>() const
        {
            constexpr auto _cast_simplex_composition =
                []<size_t... I>(
                    const oriented_simplex_composition_t<D> & oriented_composition,
                    std::index_sequence<I...>)
                    ->simplex_composition_t<D>
            {
                return simplex_composition_t<D> {
                    oriented_composition[I]->footprint().get() ... };
            };

            return _cast_simplex_composition(*this, std::make_index_sequence<D + 1> {});
        }

        constexpr bool operator==(const simplex_composition_t<D> & rhs) const requires (D > 1)
        {
            for (int i = 0; i < D; ++i)
            {
                if (this->operator[](i)->footprint().get() != rhs[i]) return false;
            }
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
        os << "footprint: " << *(s.footprint().get()) << std::endl;
        // all done
        return os;
    }
}
#endif    // mito_mesh_OrientedSimplex_h

// end of file
