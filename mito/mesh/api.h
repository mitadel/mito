// code guard
#if !defined(mito_mesh_api_h)
#define mito_mesh_api_h


namespace mito::mesh {

    // point factory
    template <int D, class... Args>
    constexpr auto point(Args &&... args);

    // vertex factory
    constexpr auto vertex();


    // segment factory
    constexpr auto segment(const std::array<vertex_t *, 2> & simplices);
    constexpr auto segment(std::array<vertex_t *, 2> && simplices);

    // triangle factory
    constexpr auto triangle(const std::array<segment_t *, 3> & simplices);
    constexpr auto triangle(std::array<segment_t *, 3> && simplices);

    // tetrahedron factory
    constexpr auto tetrahedron(const std::array<triangle_t *, 4> & simplices);
    constexpr auto tetrahedron(std::array<triangle_t *, 4> && simplices);

    // vertex set factory
    template <int D>
    constexpr auto vertex_set();

    // element set factory
    template <class elementT, int D>
    constexpr auto element_set(
        const std::vector<elementT *> & elements, const vertex_set_t<D> & vertices);
    template <class elementT, int D>
    constexpr auto element_set(
        std::vector<elementT *> && elements, const vertex_set_t<D> & vertices);

    // vertex set factory
    template <int D>
    constexpr auto mesh(std::string meshFileName);

}


#endif

// end of file
