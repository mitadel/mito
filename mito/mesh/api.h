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

    // segment factory
    oriented_simplex_t<1> * segment(const oriented_simplex_composition_t<1> & simplices);
    oriented_simplex_t<1> * segment(oriented_simplex_composition_t<1> && simplices);

    // triangle factory
    oriented_simplex_t<2> * triangle(const oriented_simplex_composition_t<2> & simplices);
    oriented_simplex_t<2> * triangle(oriented_simplex_composition_t<2> && simplices);

#if 0
    // tetrahedron factory
    oriented_simplex_t<3> * tetrahedron(const oriented_simplex_composition_t<3> & simplices);
    oriented_simplex_t<3> * tetrahedron(oriented_simplex_composition_t<3> && simplices);
#endif

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
    auto mesh(std::string meshFileName);

}


#endif

// end of file
