// code guard
#if !defined(mito_geometry_factories_h)
#define mito_geometry_factories_h


namespace mito::geometry {

    // factory for coordinates from brace-enclosed initializer list
    template <int D>
    constexpr auto coordinates(const mito::scalar_t (&&coords)[D])
    {
        return coordinates_t<D>(coords);
    }

    // point cloud factory
    template <int D>
    auto point_cloud() -> point_cloud_t<D> &
    {
        return utilities::Singleton<point_cloud_t<D>>::GetInstance();
    }

    // geometry factory
    template <int D>
    auto geometry(topology::topology_t & topology, point_cloud_t<D> & point_cloud)
        -> geometry_t<D> &
    {
        return utilities::Singleton<geometry_t<D>>::GetInstance(topology, point_cloud);
    }
}


#endif

// end of file
