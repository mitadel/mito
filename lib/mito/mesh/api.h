// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // point factory
    template <int D, class... Args>
    constexpr auto point(Args &&... args);

    // oriented simplex factory
    template <int I>
    oriented_simplex_ptr<I> oriented_simplex(const simplex_composition_t<I> & simplices);

    // vertex factory
    oriented_simplex_ptr<0> vertex();

    // vertex factory
    template <int D>
    oriented_simplex_ptr<0> vertex(point_t<D> &&);

    // segment factory
    oriented_simplex_ptr<1> segment(const simplex_composition_t<1> & simplices);

    // triangle factory
    oriented_simplex_ptr<2> triangle(const simplex_composition_t<2> & simplices);

    // tetrahedron factory
    oriented_simplex_ptr<3> tetrahedron(const simplex_composition_t<3> & simplices);

    // TOFIX: where should the implementation of these methods go?
    // to a {topology} namespace!
    template <int D>
    auto exists_flipped(const oriented_simplex_ptr<D> & oriented_simplex)
    {
        return Topology<D>::exists_flipped(oriented_simplex);
    }

    template <int D>
    auto flip(const oriented_simplex_ptr<D> & oriented_simplex)
    {
        return Topology<D>::flip(oriented_simplex);
    }

    template <int D>
    auto incidence(const oriented_simplex_ptr<D> & oriented_simplex)
    {
        return Topology<D>::incidence(oriented_simplex);
    }
}


#endif

// end of file
