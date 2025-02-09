// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    // mesh writer alias
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    using mesh_writer_t = MeshWriterVTK<meshT, coordSystemT>;

    // point cloud writer alias
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    using cloud_writer_t = PointCloudWriterVTK<cloudT, coordSystemT>;

    // field writer alias
    template <class gridWriterT, geometry::coordinate_system_c coordSystemT>
    using field_writer_t = FieldWriterVTK<gridWriterT, coordSystemT>;

    // vtk mesh writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    auto grid_writer(std::string filename, const meshT & mesh, const coordSystemT & coord_system);

    // point cloud writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto grid_writer(std::string filename, const cloudT & cloud, const coordSystemT & coord_system);

    // vtk mesh field writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    auto field_writer(std::string filename, const meshT & mesh, const coordSystemT & coord_system);

    // point cloud field writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto field_writer(
        std::string filename, const cloudT & cloud, const coordSystemT & coord_system);
}


// end of file
