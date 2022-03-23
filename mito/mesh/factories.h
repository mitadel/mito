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
    template <class... Args>
    constexpr auto vertex(Args &&... args)
    {
        return vertex_t(std::forward<Args>(args)...);
    }

    // oriented simplex factory
    template<int I>
    oriented_simplex_t<I> & oriented_simplex(const oriented_simplex_composition_t<I> & simplices)
    {
        return mito::mesh::OrientedSimplexFactory<I>::OrientedSimplex(simplices);
    }
    template <int I>
    oriented_simplex_t<I> & oriented_simplex(oriented_simplex_composition_t<I> && simplices)
    {
        return mito::mesh::OrientedSimplexFactory<I>::OrientedSimplex(std::move(simplices));
    }

    // segment factory
    oriented_simplex_t<1> & segment(const oriented_simplex_composition_t<1> & simplices)
    {
        return mito::mesh::OrientedSimplexFactory<1>::OrientedSimplex(simplices);
    }
    oriented_simplex_t<1> & segment(oriented_simplex_composition_t<1> && simplices)
    {
        return mito::mesh::OrientedSimplexFactory<1>::OrientedSimplex(std::move(simplices));
    }

    // triangle factory
    oriented_simplex_t<2> & triangle(const oriented_simplex_composition_t<2> & simplices)
    {
        return mito::mesh::OrientedSimplexFactory<2>::OrientedSimplex(simplices);
    }
    oriented_simplex_t<2> & triangle(oriented_simplex_composition_t<2> && simplices)
    {
        return mito::mesh::OrientedSimplexFactory<2>::OrientedSimplex(std::move(simplices));
    }

    // tetrahedron factory
    oriented_simplex_t<3> & tetrahedron(const oriented_simplex_composition_t<3> & simplices)
    {
        return mito::mesh::OrientedSimplexFactory<3>::OrientedSimplex(simplices);
    }
    oriented_simplex_t<3> & tetrahedron(oriented_simplex_composition_t<3> && simplices)
    {
        return mito::mesh::OrientedSimplexFactory<3>::OrientedSimplex(std::move(simplices));
    }

    // vertex set factory
    template <int D>
    constexpr auto vertex_set()
    {
        return vertex_set_t<D>();
    }

    // element set factory (from vectors)
    template <class elementT, int D>
    constexpr auto element_set(
        const std::vector<elementT *> & elements, const vertex_set_t<D> & vertices)
    {
        return element_set_t<elementT, D>(elements, vertices);
    }
    template <class elementT, int D>
    constexpr auto element_set(
        std::vector<elementT *> && elements, const vertex_set_t<D> & vertices)
    {
        return element_set_t<elementT, D>(std::move(elements), vertices);
    }

    // element set factory (from sets)
    template <class elementT, int D>
    constexpr auto element_set(
        const std::unordered_set<elementT *> & elements, const vertex_set_t<D> & vertices)
    {
        return element_set_t<elementT, D>(elements, vertices);
    }
    template <class elementT, int D>
    constexpr auto element_set(
        std::unordered_set<elementT *> && elements, const vertex_set_t<D> & vertices)
    {
        return element_set_t<elementT, D>(std::move(elements), vertices);
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
