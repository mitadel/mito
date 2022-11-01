// code guard
#if !defined(mito_topology_api_h)
#define mito_topology_api_h


namespace mito::topology {

    // vertex factory
    inline auto vertex() -> simplex_t<0>;

    // segment factory
    inline auto segment(const simplex_composition_t<1> & simplices) -> simplex_t<1>;

    // triangle factory
    inline auto triangle(const simplex_composition_t<2> & simplices) -> simplex_t<2>;

    // tetrahedron factory
    inline auto tetrahedron(const simplex_composition_t<3> & simplices) -> simplex_t<3>;

    template <int D>
    inline auto exists_flipped(const simplex_t<D> & oriented_simplex) -> auto;

    template <int D>
    inline auto flip(const simplex_t<D> & oriented_simplex) -> auto;

    template <int D>
    inline auto incidence(const simplex_t<D> & oriented_simplex) -> auto;

}


#endif

// end of file
