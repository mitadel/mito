// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

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
        auto & cloud = point_cloud<D>();
        // instantiate a point
        auto point = cloud.point();
        // place it in space
        coordinate_system.place(point, coords);
        // instantiate a vertex
        auto vertex = topology::vertex();
        // instantiate a node binding a vertex to a point
        return node_t<D>(vertex, point);
    }

    // segment factory
    template <int D>
    constexpr auto segment(const node_t<D> & node_0, const node_t<D> & node_1)
        -> geometric_simplex_t<1, D>
    requires(D >= 1)
    {
        // all done
        return geometric_simplex_t<1, D>({ node_0, node_1 });
    }

    // triangle factory
    template <int D>
    constexpr auto triangle(
        const node_t<D> & node_0, const node_t<D> & node_1, const node_t<D> & node_2)
        -> geometric_simplex_t<2, D>
    requires(D >= 2)
    {
        // all done
        return geometric_simplex_t<2, D>({ node_0, node_1, node_2 });
    }

    // tetrahedron factory
    template <int D>
    constexpr auto tetrahedron(
        const node_t<D> & node_0, const node_t<D> & node_1, const node_t<D> & node_2,
        const node_t<D> & node_3) -> geometric_simplex_t<3, D>
    requires(D >= 3)
    {
        // all done
        return geometric_simplex_t<3, D>({ node_0, node_1, node_2, node_3 });
    }
}


// end of file
