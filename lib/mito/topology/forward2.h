// code guard
#if !defined(mito_topology_forward2_h)
#define mito_topology_forward2_h


namespace mito::topology {

    // oriented simplex pointer alias
    template <int D>
    using oriented_simplex_ptr = mito::utilities::shared_ptr<oriented_simplex_t<D>>;

    // oriented simplex composition alias
    template <int D>
    using simplex_composition_t = std::array<oriented_simplex_ptr<D - 1>, D + 1>;

    // simplex alias
    template <int D>
    using simplex_t = oriented_simplex_ptr<D>;

    // vertex set alias
    using vertex_set_t = element_set_t<simplex_t<0>>;

    // vertex vector alias
    using vertex_vector_t = element_vector_t<simplex_t<0>>;
}


#endif

// end of file
