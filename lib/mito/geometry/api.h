// code guard
#if !defined(mito_geometry_api_h)
#define mito_geometry_api_h


namespace mito::geometry {

    // point factory
    template <int D, class... Args>
    constexpr auto point(Args &&... args);

    // vertex factory
    template <int D>
    oriented_simplex_ptr<0> vertex(point_t<D> &&);
}


#endif

// end of file
