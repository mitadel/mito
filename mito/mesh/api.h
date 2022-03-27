// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // point factory
    template <int D, class... Args>
    constexpr auto point(Args &&... args);

    // vertex factory
    template <class... Args>
    constexpr auto vertex(Args &&... args);

    // oriented simplex factory
    template <int I>
    oriented_simplex_t<I> & oriented_simplex(
        const std::array<std::reference_wrapper<oriented_simplex_t<I - 1>>, I + 1> & simplices);

    // segment factory
    oriented_simplex_t<1> & segment(
        const std::array<std::reference_wrapper<oriented_simplex_t<0>>, 2> & simplices);

    // triangle factory
    oriented_simplex_t<2> & triangle(
        const std::array<std::reference_wrapper<oriented_simplex_t<1>>, 3> & simplices);

    // tetrahedron factory
    oriented_simplex_t<3> & tetrahedron(
        const std::array<std::reference_wrapper<oriented_simplex_t<2>>, 4> & simplices);

    // vertex set factory
    template <int D>
    constexpr auto point_cloud();

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

    // vertex set factory
    template <int D>
    auto mesh(std::string meshFileName);

}


#endif

// end of file
