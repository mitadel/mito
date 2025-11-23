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

    // vtk node writer factory
    template <fem::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<functionSpaceT, coordSystemT>)
    auto grid_writer(
        std::string filename, const functionSpaceT & node, const coordSystemT & coord_system)
    {
        return node_writer_t<functionSpaceT, coordSystemT>(filename, node, coord_system);
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

    // vtk node field writer factory
    template <fem::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<functionSpaceT, coordSystemT>)
    auto field_writer(
        std::string filename, const functionSpaceT & node, const coordSystemT & coord_system)
    {
        return field_writer_t<node_writer_t<functionSpaceT, coordSystemT>, coordSystemT>(
            filename, node, coord_system);
    }

#ifdef WITH_PARALLEL_VTK
    // parallel vtk mesh writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    auto parallel_grid_writer(
        std::string filename, const meshT & mesh, const coordSystemT & coord_system)
    {
        return parallel_mesh_writer_t<meshT, coordSystemT>(filename, mesh, coord_system);
    }

    // parallel vtk point cloud writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto parallel_grid_writer(
        std::string filename, const cloudT & cloud, const coordSystemT & coord_system)
    {
        return parallel_cloud_writer_t<cloudT, coordSystemT>(filename, cloud, coord_system);
    }

    // parallel vtk node writer factory
    template <fem::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<functionSpaceT, coordSystemT>)
    auto parallel_grid_writer(
        std::string filename, const functionSpaceT & node, const coordSystemT & coord_system)
    {
        return parallel_node_writer_t<functionSpaceT, coordSystemT>(filename, node, coord_system);
    }

    // parallel vtk mesh field writer factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    auto parallel_field_writer(
        std::string filename, const meshT & mesh, const coordSystemT & coord_system)
    {
        return field_writer_t<parallel_mesh_writer_t<meshT, coordSystemT>, coordSystemT>(
            filename, mesh, coord_system);
    }

    // parallel vtk point cloud field writer factory
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    auto parallel_field_writer(
        std::string filename, const cloudT & cloud, const coordSystemT & coord_system)
    {
        return field_writer_t<parallel_cloud_writer_t<cloudT, coordSystemT>, coordSystemT>(
            filename, cloud, coord_system);
    }

    // parallel vtk node field writer factory
    template <fem::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<functionSpaceT, coordSystemT>)
    auto parallel_field_writer(
        std::string filename, const functionSpaceT & node, const coordSystemT & coord_system)
    {
        return field_writer_t<parallel_node_writer_t<functionSpaceT, coordSystemT>, coordSystemT>(
            filename, node, coord_system);
    }

#endif    // WITH_PARALLEL_VTK
}


// end of file
