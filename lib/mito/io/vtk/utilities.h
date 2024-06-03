// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    // the type of the grid writer depending on the type of grid and on the coordinate system
    template <class gridT, geometry::coordinate_system_c coordSystemT>
    struct grid_writer;

    // specialization to {mesh_c} case
    template <mesh::mesh_c gridT, geometry::coordinate_system_c coordSystemT>
    struct grid_writer<gridT, coordSystemT> {
        using type = mesh_writer_t<gridT, coordSystemT>;
    };

    // specialization to {point_cloud_c} case
    template <geometry::point_cloud_c gridT, geometry::coordinate_system_c coordSystemT>
    struct grid_writer<gridT, coordSystemT> {
        using type = cloud_writer_t<gridT, coordSystemT>;
    };
}


// end of file
