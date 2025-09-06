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
    using mesh_writer_t = MeshVTKWriter<meshT, coordSystemT, GridVTKWriter<meshT::dim>>;

    // point cloud writer alias
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    using cloud_writer_t = PointCloudVTKWriter<cloudT, coordSystemT, GridVTKWriter<cloudT::dim>>;

    // node writer alias
    template <fem::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<functionSpaceT, coordSystemT>)
    using node_writer_t =
        NodeVTKWriter<functionSpaceT, coordSystemT, GridVTKWriter<functionSpaceT::dim>>;

    // field writer alias
    template <class gridWriterT, geometry::coordinate_system_c coordSystemT>
    using field_writer_t = FieldVTKWriter<gridWriterT, coordSystemT>;

    // vtk mesh writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    auto grid_writer(std::string filename, const meshT & mesh, const coordSystemT & coord_system);

    // point cloud writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto grid_writer(std::string filename, const cloudT & cloud, const coordSystemT & coord_system);

    // node writer factory
    template <fem::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<functionSpaceT, coordSystemT>)
    auto grid_writer(
        std::string filename, const functionSpaceT & function_space,
        const coordSystemT & coord_system);

    // vtk mesh field writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    auto field_writer(std::string filename, const meshT & mesh, const coordSystemT & coord_system);

    // point cloud field writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto field_writer(
        std::string filename, const cloudT & cloud, const coordSystemT & coord_system);

    // node field writer factory
    template <fem::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<functionSpaceT, coordSystemT>)
    auto field_writer(
        std::string filename, const functionSpaceT & function_space,
        const coordSystemT & coord_system);

#ifdef WITH_PARALLEL_VTK
    // parallel mesh writer alias
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    using parallel_mesh_writer_t =
        MeshVTKWriter<meshT, coordSystemT, ParallelGridVTKWriter<meshT::dim>>;

    // parallel point cloud writer alias
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    using parallel_cloud_writer_t =
        PointCloudVTKWriter<cloudT, coordSystemT, ParallelGridVTKWriter<cloudT::dim>>;

    // parallel node writer alias
    template <fem::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<functionSpaceT, coordSystemT>)
    using parallel_node_writer_t =
        NodeVTKWriter<functionSpaceT, coordSystemT, ParallelGridVTKWriter<functionSpaceT::dim>>;

    // parallel vtk mesh writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    auto parallel_grid_writer(
        std::string filename, const meshT & mesh, const coordSystemT & coord_system);

    // parallel point cloud writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto parallel_grid_writer(
        std::string filename, const cloudT & cloud, const coordSystemT & coord_system);

    // parallel node writer factory
    template <fem::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<functionSpaceT, coordSystemT>)
    auto parallel_node_writer(
        std::string filename, const functionSpaceT & function_space,
        const coordSystemT & coord_system);

    // parallel vtk mesh field writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    auto parallel_field_writer(
        std::string filename, const meshT & mesh, const coordSystemT & coord_system);

    // parallel point cloud field writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto parallel_field_writer(
        std::string filename, const cloudT & cloud, const coordSystemT & coord_system);

    // parallel node field writer factory
    template <fem::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<functionSpaceT, coordSystemT>)
    auto parallel_field_writer(
        std::string filename, const functionSpaceT & function_space,
        const coordSystemT & coord_system);

#endif    // WITH_PARALLEL_VTK

}


// end of file
