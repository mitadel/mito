// code guard
#if !defined(mito_topology_api_h)
#define mito_topology_api_h


namespace mito::topology {

    // simplex alias
    template <int N>
    using simplex_t = oriented_simplex_t<N>;

    // vertex alias
    using vertex_t = unoriented_simplex_t<0>;

}


#endif

// end of file
