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
    auto grid_writer(std::string filename, const meshT & mesh, const coordSystemT & coord_system)
    {
        return mesh_writer_t<meshT, coordSystemT>(filename, mesh, coord_system);
    }

    // vtk point cloud writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto grid_writer(std::string filename, const cloudT & cloud, const coordSystemT & coord_system)
    {
        return cloud_writer_t<cloudT, coordSystemT>(filename, cloud, coord_system);
    }

    // vtk mesh field writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    auto field_writer(std::string filename, const meshT & mesh, const coordSystemT & coord_system)
    {
        return field_writer_t<mesh_writer_t<meshT, coordSystemT>, coordSystemT>(
            filename, mesh, coord_system);
    }

    // vtk point cloud field writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto field_writer(std::string filename, const cloudT & cloud, const coordSystemT & coord_system)
    {
        return field_writer_t<cloud_writer_t<cloudT, coordSystemT>, coordSystemT>(
            filename, cloud, coord_system);
    }
}


// end of file
