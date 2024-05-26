// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    // vtk mesh writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(meshT::cell_type::dim == coordSystemT::coordinates_type::dim)
    auto writer(std::string filename, const meshT & mesh, const coordSystemT & coord_system)
        -> mesh_writer_t<meshT, coordSystemT>
    {
        return MeshWriter<meshT, coordSystemT>(filename, mesh, coord_system);
    }

    // vtk point cloud writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(cloudT::dim == coordSystemT::coordinates_type::dim)
    auto writer(std::string filename, const cloudT & cloud, const coordSystemT & coord_system)
        -> cloud_writer_t<cloudT, coordSystemT>
    {
        return PointCloudWriter<cloudT, coordSystemT>(filename, cloud, coord_system);
    }
}


// end of file
