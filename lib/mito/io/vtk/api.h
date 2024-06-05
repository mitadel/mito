// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    // mesh writer alias
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(meshT::dim == coordSystemT::dim)
    using mesh_writer_t = MeshWriterVTK<meshT, coordSystemT>;

    // point cloud writer alias
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(cloudT::dim == coordSystemT::dim)
    using cloud_writer_t = PointCloudWriterVTK<cloudT, coordSystemT>;

    // field writer alias
    template <class gridT, geometry::coordinate_system_c coordSystemT>
    using field_writer_t = FieldWriterVTK<gridT, coordSystemT>;

    // vtk mesh writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(meshT::dim == coordSystemT::dim)
    auto writer(std::string filename, const meshT & mesh, const coordSystemT & coord_system)
        -> field_writer_t<meshT, coordSystemT>;

    // point cloud writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(cloudT::dim == coordSystemT::dim)
    auto writer(std::string filename, const cloudT & cloud, const coordSystemT & coord_system)
        -> field_writer_t<cloudT, coordSystemT>;
}


// end of file
