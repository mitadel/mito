// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // TOFIX: typedef mito::geometry::point_cloud_t in manifold

    // element set factory (from vectors)
    template <int D, class elementT>
    constexpr auto manifold(const topology::simplex_vector_t<elementT> & elements)
    {
        return manifold_t<elementT, D>(elements);
    }
    template <int D, class elementT>
    constexpr auto manifold(topology::simplex_vector_t<elementT> && elements)
    {
        return manifold_t<elementT, D>(std::move(elements));
    }

    // element set factory (from sets)
    template <int D, class elementT>
    constexpr auto manifold(const topology::simplex_set_t<elementT> & elements)
    {
        return manifold_t<elementT, D>(elements);
    }
    template <int D, class elementT>
    constexpr auto manifold(topology::simplex_set_t<elementT> && elements)
    {
        return manifold_t<elementT, D>(std::move(elements));
    }

}


#endif

// end of file