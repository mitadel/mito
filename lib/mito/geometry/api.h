// code guard
#if !defined(mito_geometry_api_h)
#define mito_geometry_api_h


namespace mito::geometry {

    // point cloud factory
    template <int D>
    auto point_cloud() -> mito::geometry::point_cloud_t<D>;
}


#endif

// end of file
