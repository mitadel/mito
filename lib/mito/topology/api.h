// code guard
#if !defined(mito_topology_api_h)
#define mito_topology_api_h


namespace mito::topology {

    // oriented simplex factory
    template <int I>
    simplex_t<I> oriented_simplex(const simplex_composition_t<I> & simplices);

    // vertex factory
    simplex_t<0> vertex();

    // segment factory
    simplex_t<1> segment(const simplex_composition_t<1> & simplices);

    // triangle factory
    simplex_t<2> triangle(const simplex_composition_t<2> & simplices);

    // tetrahedron factory
    simplex_t<3> tetrahedron(const simplex_composition_t<3> & simplices);

    // TOFIX: where should we park these methods?
    template <int D>
    auto exists_flipped(const simplex_t<D> & oriented_simplex)
    {
        return Topology<D>::exists_flipped(oriented_simplex);
    }

    template <int D>
    auto flip(const simplex_t<D> & oriented_simplex)
    {
        return Topology<D>::flip(oriented_simplex);
    }

    template <int D>
    auto incidence(const simplex_t<D> & oriented_simplex)
    {
        return Topology<D>::incidence(oriented_simplex);
    }
}


#endif

// end of file
