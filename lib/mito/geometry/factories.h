// code guard
#if !defined(mito_geometry_factories_h)
#define mito_geometry_factories_h


namespace mito::geometry {

    // factory for coordinates from brace-enclosed initializer list
    template <mito::geometry::CoordinateType coordT = mito::geometry::EUCLIDEAN, int D>
    constexpr auto coordinates(const mito::scalar_t (&&coords)[D])
    {
        return coordinates_t<D, coordT>(coords);
    }

    // point cloud factory
    template <int D, CoordinateType coordT = mito::geometry::EUCLIDEAN>
    auto point_cloud() -> point_cloud_t<D, coordT> &
    {
        return utilities::Singleton<point_cloud_t<D, coordT>>::GetInstance();
    }

    // geometry factory
    template <int D, CoordinateType coordT = mito::geometry::EUCLIDEAN>
    auto geometry(topology::topology_t & topology, point_cloud_t<D, coordT> & point_cloud)
        -> geometry_t<D, coordT> &
    {
        return utilities::Singleton<geometry_t<D, coordT>>::GetInstance(topology, point_cloud);
    }
}


#endif

// end of file
