// code guard
#if !defined(mito_topology_factories_h)
#define mito_topology_factories_h


namespace mito::topology {

    // oriented simplex factory
    template <int I>
    inline simplex_t<I> oriented_simplex(const simplex_composition_t<I> & simplices)
    {
        return Topology<I>::orientedSimplex(simplices);
    }

    // vertex factory
    simplex_t<0> vertex() { return Topology<0>::orientedSimplex(); }

    // segment factory
    simplex_t<1> segment(const simplex_composition_t<1> & simplices)
    {
        return oriented_simplex<1>(simplices);
    }

    // triangle factory
    simplex_t<2> triangle(const simplex_composition_t<2> & simplices)
    {
        return oriented_simplex<2>(simplices);
    }

    // tetrahedron factory
    simplex_t<3> tetrahedron(const simplex_composition_t<3> & simplices)
    {
        return oriented_simplex<3>(simplices);
    }
}


#endif

// end of file
