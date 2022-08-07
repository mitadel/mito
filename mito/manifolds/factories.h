// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // TOFIX: typedef mito::mesh::point_cloud_t in manifold

    // element set factory (from vectors)
    template <class elementT, int D>
    constexpr auto manifold(
        const mesh::simplex_vector_t<elementT> & elements, const mesh::point_cloud_t<D> & vertices)
    {
        return manifold_t<elementT, D>(elements, vertices);
    }
    template <class elementT, int D>
    constexpr auto manifold(
        mesh::simplex_vector_t<elementT> && elements, const mesh::point_cloud_t<D> & vertices)
    {
        return manifold_t<elementT, D>(std::move(elements), vertices);
    }

    // element set factory (from sets)
    template <class elementT, int D>
    constexpr auto manifold(
        const mesh::simplex_set_t<elementT> & elements, const mesh::point_cloud_t<D> & vertices)
    {
        return manifold_t<elementT, D>(elements, vertices);
    }
    template <class elementT, int D>
    constexpr auto manifold(
        mesh::simplex_set_t<elementT> && elements, const mesh::point_cloud_t<D> & vertices)
    {
        return manifold_t<elementT, D>(std::move(elements), vertices);
    }

}


#endif

// end of file
