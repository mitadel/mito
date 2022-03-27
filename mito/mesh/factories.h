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
    template <int I>
    oriented_simplex_t<I> & oriented_simplex(
        const std::array<std::reference_wrapper<oriented_simplex_t<I - 1>>, I + 1> & simplices)
    {
        // helper function
        constexpr oriented_simplex_t<I> & _oriented_simplex = []<size_t... K>(
            const std::array<std::reference_wrapper<oriented_simplex_t<I - 1>>, I + 1> & simplices,
            std::index_sequence<K...>) consteval
        {
            return mito::mesh::OrientedSimplexFactory<I>::OrientedSimplex(
                oriented_simplex_composition_t<I> { (&simplices[K].get())... });
        };

        return _oriented_simplex(simplices, std::make_index_sequence<I + 1> {});
    }

    // segment factory
    oriented_simplex_t<1> & segment(
        const std::array<std::reference_wrapper<oriented_simplex_t<0>>, 2> & simplices)
    {
        return mito::mesh::OrientedSimplexFactory<1>::OrientedSimplex(
            oriented_simplex_composition_t<1> { &simplices[0].get(), &simplices[1].get() });
    }

    // triangle factory
    oriented_simplex_t<2> & triangle(
        const std::array<std::reference_wrapper<oriented_simplex_t<1>>, 3> & simplices)
    {
        return mito::mesh::OrientedSimplexFactory<2>::OrientedSimplex(
            oriented_simplex_composition_t<2> { &simplices[0].get(), &simplices[1].get(),
                                                &simplices[2].get() });
    }

    // tetrahedron factory
    oriented_simplex_t<3> & tetrahedron(
        const std::array<std::reference_wrapper<oriented_simplex_t<2>>, 4> & simplices)
    {
        return mito::mesh::OrientedSimplexFactory<3>::OrientedSimplex(
            oriented_simplex_composition_t<3> { &simplices[0].get(), &simplices[1].get(),
                                                &simplices[2].get(), &simplices[3].get() });
    }

    // vertex set factory
    template <int D>
    constexpr auto point_cloud()
    {
        return point_cloud_t<D>();
    }

    // element set factory (from vectors)
    template <class elementT, int D>
    constexpr auto element_set(
        const std::vector<elementT *> & elements, const point_cloud_t<D> & vertices)
    {
        return element_set_t<elementT, D>(elements, vertices);
    }
    template <class elementT, int D>
    constexpr auto element_set(
        std::vector<elementT *> && elements, const point_cloud_t<D> & vertices)
    {
        return element_set_t<elementT, D>(std::move(elements), vertices);
    }

    // element set factory (from sets)
    template <class elementT, int D>
    constexpr auto element_set(
        const std::unordered_set<elementT *> & elements, const point_cloud_t<D> & vertices)
    {
        return element_set_t<elementT, D>(elements, vertices);
    }
    template <class elementT, int D>
    constexpr auto element_set(
        std::unordered_set<elementT *> && elements, const point_cloud_t<D> & vertices)
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
