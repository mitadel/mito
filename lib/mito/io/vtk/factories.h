// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    // vtk mesh writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    auto writer(std::string filename, const meshT & mesh, const coordSystemT & coord_system)
        -> field_writer_t<meshT, coordSystemT>
    {
        return FieldWriterVTK<meshT, coordSystemT>(filename, mesh, coord_system);
    }

    // vtk point cloud writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto writer(std::string filename, const cloudT & cloud, const coordSystemT & coord_system)
        -> field_writer_t<cloudT, coordSystemT>
    {
        return FieldWriterVTK<cloudT, coordSystemT>(filename, cloud, coord_system);
    }
}


// end of file
