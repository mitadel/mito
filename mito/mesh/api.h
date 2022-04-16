// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // point factory
    template <int D, class... Args>
    constexpr auto point(Args &&... args);

    // vertex factory
    constexpr auto vertex();

    // oriented simplex factory
    template <int I>
    std::shared_ptr<const oriented_simplex_t<I>> oriented_simplex(
        const simplex_composition_t<I> & simplices) requires(I > 1);

    // segment factory
    std::shared_ptr<const oriented_simplex_t<1>> segment(
        const std::array<std::reference_wrapper<oriented_simplex_t<0>>, 2> & simplices);

    // triangle factory
    std::shared_ptr<const oriented_simplex_t<2>> triangle(
        const simplex_composition_t<2> & simplices);

    // tetrahedron factory
    std::shared_ptr<const oriented_simplex_t<3>> tetrahedron(
        const simplex_composition_t<3> & simplices);

    // point cloud factory
    template <int D>
    constexpr auto point_cloud();

    // mesh factory
    template <int D>
    auto mesh(std::string meshFileName);

}


#endif

// end of file
