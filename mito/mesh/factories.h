// code guard
#if !defined(mito_mesh_factories_h)
#define mito_mesh_factories_h


namespace mito::mesh {

    // point factory
    template <int D, class... Args>
    constexpr auto point(Args &&... args)
    {
        return point_t<D>(std::forward<Args>(args)...);
    }

    // vertex factory
    constexpr auto vertex() { return vertex_t(); }

    // oriented simplex factory
    template <int I>
    oriented_simplex_ptr<I> oriented_simplex(const simplex_composition_t<I> & simplices) requires(
        I > 1)
    {
        return OrientedSimplexFactory<I>::OrientedSimplex(simplices);
    }

    // segment factory
    oriented_simplex_ptr<1> segment(
        const std::array<std::reference_wrapper<oriented_simplex_t<0>>, 2> & simplices)
    {
        return OrientedSimplexFactory<1>::OrientedSimplex(
            simplex_composition_t<1> { &simplices[0].get(), &simplices[1].get() });
    }

    // triangle factory
    oriented_simplex_ptr<2> triangle(const simplex_composition_t<2> & simplices)
    {
        return oriented_simplex<2>(simplices);
    }

    // tetrahedron factory
    oriented_simplex_ptr<3> tetrahedron(const simplex_composition_t<3> & simplices)
    {
        return oriented_simplex<3>(simplices);
    }

    // vertex set factory
    template <int D>
    constexpr auto point_cloud()
    {
        return point_cloud_t<D>();
    }

    // vertex set factory
    template <int D>
    auto mesh(std::string meshFileName)
    {
        return mesh_t<D>(meshFileName);
    }
}


#endif

// end of file
