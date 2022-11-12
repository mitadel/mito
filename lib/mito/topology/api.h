// code guard
#if !defined(mito_topology_api_h)
#define mito_topology_api_h


namespace mito::topology {

    // vertex alias
    using vertex_t = simplex_t<0>;

    // segment alias
    using segment_t = simplex_t<1>;

    // triangle alias
    using triangle_t = simplex_t<2>;

    // tetrahedron alias
    using tetrahedron_t = simplex_t<3>;

    // vertex set alias
    using vertex_set_t = element_set_t<vertex_t>;

    // vertex vector alias
    using vertex_vector_t = element_vector_t<vertex_t>;

    template <int D>
    inline auto exists_flipped(const simplex_t<D> & oriented_simplex) -> auto;

    template <int D>
    inline auto flip(const simplex_t<D> & oriented_simplex) -> auto;

    template <int D>
    inline auto incidence(const simplex_t<D> & oriented_simplex) -> auto;

}


#endif

// end of file
