// code guard
#if !defined(mito_topology_forward2_h)
#define mito_topology_forward2_h


namespace mito::topology {

    // alias for oriented simplex recursive composition
    // (e.g. a D-simplex has D+1 (D-1)-simplices)
    template <int D>
    using simplex_composition_t = std::array<oriented_simplex_t<D - 1>, D + 1>;

    // alias for oriented simplex composition in terms of vertices
    // (e.g. a D-simplex has D + 1 0-simplices, i.e. vertices)
    template <int D>
    using vertex_simplex_composition_t = std::array<vertex_t, D + 1>;

    // vertex set alias
    using vertex_set_t = element_set_t<vertex_t>;

    // vertex vector alias
    using vertex_vector_t = element_vector_t<vertex_t>;
}


#endif

// end of file
