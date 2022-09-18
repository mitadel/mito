// code guard
#if !defined(mito_geometry_factories_h)
#define mito_geometry_factories_h


namespace mito::geometry {

    // point factory
    template <int D, class... Args>
    constexpr auto point(Args &&... args)
    {
        return point_t<D>(std::forward<Args>(args)...);
    }

    template <int D>
    oriented_simplex_ptr<0> vertex(point_t<D> && point)
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
