// code guard
#if !defined(mito_topology_BadSimplex_h)
#define mito_topology_BadSimplex_h

namespace mito::topology {

    /*
     * This class is a sentinel for a bad simplex.
     */

    template <int D>
    class BadSimplex : public OrientedSimplex<D> {
      public:
        using simplex_t = OrientedSimplex<D>;

        // private constructor and destructor: only the SimplexFactory has the right to instantiate
        // simplices
      private:
        // constructor for a bad
        constexpr BadSimplex() : simplex_t() {}

        // destructor
        constexpr ~BadSimplex() override {}

      private:
        // delete copy constructor
        BadSimplex(const BadSimplex &) = delete;

        // delete move constructor
        BadSimplex(BadSimplex &&) = delete;

        // delete assignment operator
        BadSimplex & operator=(const BadSimplex &) = delete;

        // delete move assignment operator
        BadSimplex & operator=(BadSimplex &&) = delete;

      private:
        inline auto _bad_simplex_exception()
        {
            pyre::journal::error_t error("BadSegment");
            error << "Bad Segment!" << pyre::journal::endl;
            throw std::runtime_error("Bad Segment");
        }

      public:
        // accessor for the unoriented footprint
        inline auto footprint() const noexcept -> const unoriented_simplex_t<D> &
        {
            // throw bad simplex exception
            return _bad_simplex_exception();
        }

        // returns the orientation of this simplex
        inline auto orientation() const noexcept -> bool
        {
            // throw bad simplex exception
            return _bad_simplex_exception();
        }

        // returns the array of subsimplices
        inline auto composition() const noexcept -> const simplex_composition_t<D> &
        {
            // throw bad simplex exception
            return _bad_simplex_exception();
        }

        // returns the set of vertices
        template <class VERTEX_COLLECTION_T>
        inline auto vertices(VERTEX_COLLECTION_T & vertices) const -> void
        {
            // throw bad simplex exception
            return _bad_simplex_exception();
        }

        // returns whether the simplex passes the sanity check
        inline auto sanityCheck() const -> bool
        {
            // throw bad simplex exception
            return _bad_simplex_exception();
        }

      private:
        inline auto _erase() -> void
        {
            // throw bad simplex exception
            return _bad_simplex_exception();
        }

      private:
        // private friendship with the factory of oriented simplices
        friend class OrientedSimplexFactory<D>;
    };

}    // namespace mito


#endif    // mito_topology_Simplex_h

// end of file
