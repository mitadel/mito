// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // element set factory (from vectors)
    template <class elementT, int D>
    constexpr auto element_set(
        const std::vector<elementT *> & elements, const mito::mesh::point_cloud_t<D> & vertices)
    {
        return element_set_t<elementT, D>(elements, vertices);
    }
    template <class elementT, int D>
    constexpr auto element_set(
        std::vector<elementT *> && elements, const mito::mesh::point_cloud_t<D> & vertices)
    {
        return element_set_t<elementT, D>(std::move(elements), vertices);
    }

    // element set factory (from sets)
    template <class elementT, int D>
    constexpr auto element_set(
        const std::unordered_set<elementT *> & elements,
        const mito::mesh::point_cloud_t<D> & vertices)
    {
        return element_set_t<elementT, D>(elements, vertices);
    }
    template <class elementT, int D>
    constexpr auto element_set(
        std::unordered_set<elementT *> && elements, const mito::mesh::point_cloud_t<D> & vertices)
    {
        return element_set_t<elementT, D>(std::move(elements), vertices);
    }

}


#endif

// end of file
