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
    oriented_simplex_t<I> & oriented_simplex(
        const std::array<std::reference_wrapper<oriented_simplex_t<I - 1>>, I + 1> & simplices)
        requires (I > 1)
    {
        // helper function
        constexpr auto _composition =
            []<size_t... K>(
                const std::array<std::reference_wrapper<oriented_simplex_t<I - 1>>, I + 1> &
                    simplices,
                std::index_sequence<K...>)
                ->simplex_composition_t<I>
        {
            return { (simplices[K].get())... };
        };

        return OrientedSimplexFactory<I>::OrientedSimplex(
            _composition(simplices, std::make_index_sequence<I + 1> {}));
    }

    // segment factory
    oriented_simplex_t<1> & segment(
        const std::array<std::reference_wrapper<oriented_simplex_t<0>>, 2> & simplices)
    {
        return OrientedSimplexFactory<1>::OrientedSimplex(
            simplex_composition_t<1> { &simplices[0].get(), &simplices[1].get() });
    }

    // triangle factory
    oriented_simplex_t<2> & triangle(
        const std::array<std::reference_wrapper<oriented_simplex_t<1>>, 3> & simplices)
    {
        return oriented_simplex<2>(simplices);
    }

    // tetrahedron factory
    oriented_simplex_t<3> & tetrahedron(
        const std::array<std::reference_wrapper<oriented_simplex_t<2>>, 4> & simplices)
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
