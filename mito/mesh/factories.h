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

    // oriented simplex factory
    template <int I>
    oriented_simplex_ptr<I> oriented_simplex(const simplex_composition_t<I> & simplices)
    {
        return OrientedSimplexFactory<I>::orientedSimplex(simplices);
    }

    // vertex factory
    oriented_simplex_ptr<0> vertex() { return std::make_shared<oriented_simplex_t<0>>(); }

    // segment factory
    oriented_simplex_ptr<1> segment(const simplex_composition_t<1> & simplices)
    {
        return oriented_simplex<1>(simplices);
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

    // points cloud factory
    template <int D>
    constexpr auto point_cloud()
    {
        return point_cloud_t<D>();
    }

    // mesh factory
    template <int D>
    auto mesh(std::string meshFileName)
    {
        return mesh_t<D>(meshFileName);
    }
}


#endif

// end of file
