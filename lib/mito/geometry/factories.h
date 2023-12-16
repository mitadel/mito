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

    // factory for coordinate system
    template <int D, mito::geometry::CoordinateType coordT = mito::geometry::EUCLIDEAN>
    constexpr auto coordinate_system()
    {
        return coordinate_system_t<D, coordT>();
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

    // node factory
    template <int D, mito::geometry::CoordinateType coordT>
    constexpr auto node(
        geometry_t<D> & geometry, coordinate_system_t<D, coordT> & coordinate_system,
        const coordinates_t<D, coordT> & coords) -> topology::vertex_t
    {
        // create and place two points
        auto point = geometry.point_cloud().point();
        coordinate_system.place(point, coords);
        return geometry.node(point);
    }
}


#endif

// end of file
