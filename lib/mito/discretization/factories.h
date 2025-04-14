// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // quadrature field factory
    template <class Y, int Q, mesh::mesh_c meshT>
    constexpr auto quadrature_field(const meshT & mesh, std::string name)
    {
        // build a quadrature field on the cells collected from the mesh
        // ({Q} quad points per cell)
        return quadrature_field_t<typename meshT::cell_type::simplex_type, Q, Y>(
            mesh.cells(), name);
    }

    // nodal field factory
    template <class Y, mesh::mesh_c meshT>
    constexpr auto nodal_field(const meshT & mesh, std::string name)
    {
        // assemble the node type
        using node_type = geometry::node_t<meshT::dim>;

        // get the nodes in the mesh
        std::unordered_set<node_type, utilities::hash_function<node_type>> nodes;
        mesh::get_nodes(mesh, nodes);

        // build a nodal field on the nodes collected from the mesh
        return nodal_field_t<meshT::dim, Y>(nodes, name);
    }

    // point field factory
    template <class Y, geometry::point_cloud_c cloudT>
    constexpr auto point_field(const cloudT & cloud, std::string name)
    {
        // build a point field on the points collected from the cloud
        return point_field_t<cloudT::dim, Y>(cloud.points(), name);
    }

    // isoparametric simplex factory
    template <class geometricSimplexT>
    constexpr auto isoparametric_simplex()
    {
        // build an isoparametric simplex and return it
        return isoparametric_simplex_t<geometricSimplexT>();
    }

    // function space factory
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    constexpr auto function_space(const meshT & mesh, const coordSystemT & coord_system)
    {
        // build a function space on the mesh and return it
        return function_space_t<meshT, coordSystemT>(mesh, coord_system);
    }
}


// end of file
