// code guard
#if !defined(mito_geometry_factories_h)
#define mito_geometry_factories_h


namespace mito::geometry {

    // point factory
    template <class... Args>
    constexpr auto point(Args &&... args) -> point_t<sizeof...(Args)>
    {
        return point_t<sizeof...(Args)>(std::forward<Args>(args)...);
    }

    template <int D>
    auto vertex(point_t<D> && point) -> mito::topology::vertex_t
    {
        // get a new vertex from the topology
        auto new_vertex = mito::topology::vertex();
        // register the new vertex with the point cloud
        point_cloud<D>::insert(new_vertex, point);
        // return the new vertex
        return new_vertex;
    }
}


#endif

// end of file
