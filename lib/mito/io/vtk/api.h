// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    // mesh writer alias
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(meshT::cell_type::dim == coordSystemT::coordinates_type::dim)
    using mesh_writer_t = MeshWriterVTK<meshT, coordSystemT>;

    // vtk mesh writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(meshT::cell_type::dim == coordSystemT::coordinates_type::dim)
    auto writer(std::string filename, const meshT & mesh, const coordSystemT & coord_system)
        -> mesh_writer_t<meshT, coordSystemT>;

    // point cloud writer alias
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(cloudT::dim == coordSystemT::coordinates_type::dim)
    using cloud_writer_t = PointCloudWriterVTK<cloudT, coordSystemT>;

    // point cloud writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(cloudT::dim == coordSystemT::coordinates_type::dim)
    auto writer(std::string filename, const cloudT & cloud, const coordSystemT & coord_system)
        -> cloud_writer_t<cloudT, coordSystemT>;
}


// end of file
