// code guard
#if !defined(mito_geometry_factories_h)
#define mito_geometry_factories_h


namespace mito::geometry {

    // point cloud factory
    template <int D>
    auto point_cloud() -> point_cloud_t<D> &
    {
        return mito::utilities::Singleton<point_cloud_t<D>>::GetInstance();
    }

    // geometry factory
    template <int D>
    auto geometry(mito::topology::topology_t & topology, point_cloud_t<D> & point_cloud)
        -> geometry_t<D> &
    {
        return mito::utilities::Singleton<Geometry<D>>::GetInstance(topology, point_cloud);
    }
}


#endif

// end of file
