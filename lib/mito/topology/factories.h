// code guard
#if !defined(mito_topology_factories_h)
#define mito_topology_factories_h


namespace mito::topology {

    // oriented simplex factory
    template <int I>
    inline auto oriented_simplex(const simplex_composition_t<I> & simplices) -> simplex_t<I>
    {
        return Topology<I>::orientedSimplex(simplices);
    }

    // vertex factory
    auto vertex() -> simplex_t<0> { return Topology<0>::orientedSimplex(); }

    // segment factory
    auto segment(const simplex_composition_t<1> & simplices) -> simplex_t<1>
    {
        return oriented_simplex<1>(simplices);
    }

    // triangle factory
    auto triangle(const simplex_composition_t<2> & simplices) -> simplex_t<2>
    {
        return oriented_simplex<2>(simplices);
    }

    // tetrahedron factory
    auto tetrahedron(const simplex_composition_t<3> & simplices) -> simplex_t<3>
    {
        return oriented_simplex<3>(simplices);
    }
}


#endif

// end of file
