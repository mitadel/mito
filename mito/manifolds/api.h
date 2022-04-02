// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::manifolds {

    // element set factory (from vectors)
    template <class elementT, int D>
    constexpr auto manifold(
        const mito::mesh::simplex_vector_t<elementT> & elements,
        const mito::mesh::point_cloud_t<D> & points);
    template <class elementT, int D>
    constexpr auto manifold(
        mito::mesh::simplex_vector_t<elementT> && elements,
        const mito::mesh::point_cloud_t<D> & points);

    // element set factory (from sets)
    template <class elementT, int D>
    constexpr auto manifold(
        const mito::mesh::simplex_set_t<elementT> & elements,
        const mito::mesh::point_cloud_t<D> & points);
    template <class elementT, int D>
    constexpr auto manifold(
        mito::mesh::simplex_set_t<elementT> && elements, const mito::mesh::point_cloud_t<D> & points);
}


#endif

// end of file
