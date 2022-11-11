// code guard
#if !defined(mito_topology_api_h)
#define mito_topology_api_h


namespace mito::topology {

    template <int D>
    inline auto exists_flipped(const simplex_t<D> & oriented_simplex) -> auto;

    template <int D>
    inline auto flip(const simplex_t<D> & oriented_simplex) -> auto;

    template <int D>
    inline auto incidence(const simplex_t<D> & oriented_simplex) -> auto;

}


#endif

// end of file
