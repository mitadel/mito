// code guard
#if !defined(mito_geometry_api_h)
#define mito_geometry_api_h


namespace mito::geometry {

    // point factory
    template <int D, class... Args>
    constexpr auto point(Args &&... args) -> point_t<D>;

    // vertex factory
    template <int D>
    inline auto vertex(point_t<D> &&) -> vertex_t;
}


#endif

// end of file
