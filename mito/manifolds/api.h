// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::manifolds {

    // element set factory (from vectors)
    template <class elementT, int D>
    constexpr auto manifold(
        const std::vector<elementT *> & elements, const mito::mesh::point_cloud_t<D> & points);
    template <class elementT, int D>
    constexpr auto manifold(
        std::vector<elementT *> && elements, const mito::mesh::point_cloud_t<D> & points);

    // element set factory (from sets)
    template <class elementT, int D>
    constexpr auto manifold(
        const std::unordered_set<elementT *> & elements,
        const mito::mesh::point_cloud_t<D> & points);
    template <class elementT, int D>
    constexpr auto manifold(
        std::unordered_set<elementT *> && elements, const mito::mesh::point_cloud_t<D> & points);
}


#endif

// end of file
