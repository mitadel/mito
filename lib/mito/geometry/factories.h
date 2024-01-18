// code guard
#if !defined(mito_geometry_factories_h)
#define mito_geometry_factories_h


namespace mito::geometry {

    // factory for coordinates from brace-enclosed initializer list (case D > 1)
    template <CoordinateType coordT = EUCLIDEAN, int D>
    constexpr auto coordinates(const mito::scalar_t (&&coords)[D])
    requires(D > 1)
    {
        return coordinates_t<D, coordT>(coords);
    }

    // factory for coordinates from brace-enclosed initializer list (case D = 1)
    template <CoordinateType coordT = EUCLIDEAN>
    constexpr auto coordinates(const mito::scalar_t && coords)
    {
        return coordinates_t<1, coordT>(coords);
    }

    // factory for coordinate system
    template <int D, CoordinateType coordT = EUCLIDEAN>
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

    // node factory
    template <int D, CoordinateType coordT>
    constexpr auto node(
        coordinate_system_t<D, coordT> & coordinate_system, const coordinates_t<D, coordT> & coords)
        -> node_t<D>
    {
        // fetch the topology
        auto & topology = mito::topology::topology();
        // fetch the point cloud
        auto & point_cloud = mito::geometry::point_cloud<D>();
        // instantiate a point
        auto point = point_cloud.point();
        // place it in space
        coordinate_system.place(point, coords);
        // instantiate a vertex
        auto vertex = topology.vertex();
        // instantiate a node binding a vertex to a point
        return node_t<D>(vertex, point);
    }

    // segment factory
    template <int D>
    constexpr auto segment(typename geometric_simplex_t<1, D>::nodes_type && nodes)
        -> geometric_simplex_t<1, D>
    requires(D >= 1)
    {
        // fetch the topology
        auto & topology = mito::topology::topology();

        // fetch vertices
        auto vertex_0 = nodes[0].vertex();
        auto vertex_1 = nodes[1].vertex();

        // instantiate a segment
        const auto & simplex = topology.segment({ vertex_0, vertex_1 });

        // all done
        return geometric_simplex_t<1, D>(simplex, nodes);
    }

    // triangle factory
    template <int D>
    constexpr auto triangle(typename geometric_simplex_t<2, D>::nodes_type && nodes)
        -> geometric_simplex_t<2, D>
    requires(D >= 2)
    {
        // fetch the topology
        auto & topology = mito::topology::topology();

        // fetch vertices
        auto vertex_0 = nodes[0].vertex();
        auto vertex_1 = nodes[1].vertex();
        auto vertex_2 = nodes[2].vertex();

        // instantiate a triangle
        const auto & simplex = topology.triangle({ vertex_0, vertex_1, vertex_2 });

        // all done
        return geometric_simplex_t<2, D>(simplex, nodes);
    }

    // tetrahedron factory
    template <int D>
    constexpr auto tetrahedron(typename geometric_simplex_t<3, D>::nodes_type && nodes)
        -> geometric_simplex_t<3, D>
    requires(D >= 3)
    {
        // fetch the topology
        auto & topology = mito::topology::topology();

        // fetch vertices
        auto vertex_0 = nodes[0].vertex();
        auto vertex_1 = nodes[1].vertex();
        auto vertex_2 = nodes[2].vertex();
        auto vertex_3 = nodes[3].vertex();

        // instantiate a tetrahedron
        const auto & simplex = topology.tetrahedron({ vertex_0, vertex_1, vertex_2, vertex_3 });

        // all done
        return geometric_simplex_t<3, D>(simplex, nodes);
    }
}


#endif

// end of file
