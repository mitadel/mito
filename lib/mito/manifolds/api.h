// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::manifolds {

    // element set factory (from vectors)
    template <int D, class elementT>
    constexpr auto manifold(const topology::simplex_vector_t<elementT> & elements);
    template <int D, class elementT>
    constexpr auto manifold(topology::simplex_vector_t<elementT> && elements);

    // element set factory (from sets)
    template <int D, class elementT>
    constexpr auto manifold(const topology::simplex_set_t<elementT> & elements);
    template <int D, class elementT>
    constexpr auto manifold(topology::simplex_set_t<elementT> && elements);
}


#endif

// end of file
