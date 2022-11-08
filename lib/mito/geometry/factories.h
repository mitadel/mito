// code guard
#if !defined(mito_geometry_factories_h)
#define mito_geometry_factories_h


namespace mito::geometry {

    // TOFIX: adjust it to the new design
    // template <int D>
    // auto vertex(point_t<D> && point) -> mito::topology::vertex_t
    // {
    //     // get a new vertex from the topology
    //     auto new_vertex = mito::topology::vertex();
    //     // register the new vertex with the point cloud
    //     point_cloud<D>::insert(new_vertex, point);
    //     // return the new vertex
    //     return new_vertex;
    // }

    // point cloud factory
    template <int D>
    auto point_cloud() -> mito::geometry::point_cloud_t<D>
    {
        return mito::geometry::point_cloud_t<D>();
    }
}


#endif

// end of file
