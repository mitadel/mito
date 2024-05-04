// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_factories_h)
#define mito_geometry_factories_h


namespace mito::geometry {

    // factory for coordinates from brace-enclosed initializer list
    template <coordinates_c coordT>
    constexpr auto coordinates(mito::scalar_t (&&coords)[coordT::dim])
    {
        return coordT(std::move(coords));
    }

    // factory for coordinate system
    template <coordinates_c coordT>
    constexpr auto coordinate_system()
    {
        return coordinate_system_t<coordT>();
    }

    // factory for building a new coordinate system of coordinate type {coordT2} from an existing
    // coordinate system of coordinate type {coordT1}
    template <coordinates_c coordT2, coordinates_c coordT1>
    constexpr auto coordinate_system(const coordinate_system_t<coordT1> & coord_sys)
        -> coordinate_system_t<coordT2>
    {
        return coordinate_system_t<coordT2>(coord_sys);
    }

    // point cloud factory
    template <int D>
    auto point_cloud() -> point_cloud_t<D> &
    {
        return utilities::Singleton<point_cloud_t<D>>::GetInstance();
    }

    // node factory
    template <coordinates_c coordT>
    constexpr auto node(coordinate_system_t<coordT> & coordinate_system, const coordT & coords)
        -> node_t<coordT::dim>
    {
        // the dimension of the physical space
        constexpr int D = coordT::dim;
        // fetch the point cloud
        auto & point_cloud = mito::geometry::point_cloud<D>();
        // instantiate a point
        auto point = point_cloud.point();
        // place it in space
        coordinate_system.place(point, coords);
        // instantiate a vertex
        auto vertex = mito::topology::vertex();
        // instantiate a node binding a vertex to a point
        return node_t<D>(vertex, point);
    }

    // segment factory
    template <int D>
    constexpr auto segment(typename geometric_simplex_t<1, D>::nodes_type && nodes)
        -> geometric_simplex_t<1, D>
    requires(D >= 1)
    {
        // all done
        return geometric_simplex_t<1, D>(std::move(nodes));
    }

    // triangle factory
    template <int D>
    constexpr auto triangle(typename geometric_simplex_t<2, D>::nodes_type && nodes)
        -> geometric_simplex_t<2, D>
    requires(D >= 2)
    {
        // all done
        return geometric_simplex_t<2, D>(std::move(nodes));
    }

    // tetrahedron factory
    template <int D>
    constexpr auto tetrahedron(typename geometric_simplex_t<3, D>::nodes_type && nodes)
        -> geometric_simplex_t<3, D>
    requires(D >= 3)
    {
        // all done
        return geometric_simplex_t<3, D>(std::move(nodes));
    }
}


#endif

// end of file
