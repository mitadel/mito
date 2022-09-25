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

    // TOFIX: where should we park these methods?
    template <int D>
    inline auto exists_flipped(const simplex_t<D> & oriented_simplex) -> auto
    {
        return Topology<D>::exists_flipped(oriented_simplex);
    }

    template <int D>
    inline auto flip(const simplex_t<D> & oriented_simplex) -> auto
    {
        return Topology<D>::flip(oriented_simplex);
    }

    template <int D>
    inline auto incidence(const simplex_t<D> & oriented_simplex) -> auto
    {
        return Topology<D>::incidence(oriented_simplex);
    }
}


#endif

// end of file
