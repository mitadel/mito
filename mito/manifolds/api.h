// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::mesh {

    // element set factory (from vectors)
    template <class elementT, int D>
    constexpr auto element_set(
        const std::vector<elementT *> & elements, const point_cloud_t<D> & points);
    template <class elementT, int D>
    constexpr auto element_set(
        std::vector<elementT *> && elements, const point_cloud_t<D> & points);

    // element set factory (from sets)
    template <class elementT, int D>
    constexpr auto element_set(
        const std::unordered_set<elementT *> & elements, const point_cloud_t<D> & points);
    template <class elementT, int D>
    constexpr auto element_set(
        std::unordered_set<elementT *> && elements, const point_cloud_t<D> & points);

}


#endif

// end of file
