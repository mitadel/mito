// code guard
#if !defined(mito_topology_forward2_h)
#define mito_topology_forward2_h


namespace mito::topology {

    // oriented simplex composition alias
    template <int D>
    using simplex_composition_t = std::array<oriented_simplex_ptr<D - 1>, D + 1>;

    // vertex set alias
    using vertex_set_t = element_set_t<simplex_t<0>>;

    // vertex vector alias
    using vertex_vector_t = element_vector_t<simplex_t<0>>;
}


#endif

// end of file
