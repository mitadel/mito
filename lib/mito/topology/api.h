// code guard
#if !defined(mito_topology_api_h)
#define mito_topology_api_h


namespace mito::topology {

    // vertex factory
    inline simplex_t<0> vertex();

    // segment factory
    inline simplex_t<1> segment(const simplex_composition_t<1> & simplices);

    // triangle factory
    inline simplex_t<2> triangle(const simplex_composition_t<2> & simplices);

    // tetrahedron factory
    inline simplex_t<3> tetrahedron(const simplex_composition_t<3> & simplices);

    // TOFIX: where should we park these methods?
    template <int D>
    inline auto exists_flipped(const simplex_t<D> & oriented_simplex)
    {
        return Topology<D>::exists_flipped(oriented_simplex);
    }

    template <int D>
    inline auto flip(const simplex_t<D> & oriented_simplex)
    {
        return Topology<D>::flip(oriented_simplex);
    }

    template <int D>
    inline auto incidence(const simplex_t<D> & oriented_simplex)
    {
        return Topology<D>::incidence(oriented_simplex);
    }
}


#endif

// end of file
