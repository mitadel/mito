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

    // mesh field factory
    template <class Y, mesh::mesh_c meshT>
    constexpr auto mesh_field(const meshT & mesh, std::string name)
    {
        // assemble the node type
        using node_type = geometry::node_t<meshT::dim>;

        // get the nodes in the mesh
        std::unordered_set<node_type, utilities::hash_function<node_type>> nodes;
        mesh::get_nodes(mesh, nodes);

        // build a mesh field on the nodes collected from the mesh
        return mesh_field_t<meshT::dim, Y>(nodes, name);
    }

    // mesh field factory from a continuous field
    template <fields::field_c fieldT>
    constexpr auto mesh_field(
        const mesh::mesh_c auto & mesh, const geometry::coordinate_system_c auto & coord_system,
        const fieldT & field, std::string name)
    {
        // create a mesh field on the mesh
        auto m_field = mesh_field<typename fieldT::output_type>(mesh, name);

        // populate the mesh field with the values of the continuous field
        for (auto & [node, value] : m_field) {
            // get the position of {node}
            auto coord = coord_system.coordinates(node->point());
            // evaluate the continuousfield at {coord}
            value = field(coord);
        }

        // return the mesh field
        return m_field;
    }

    // point field factory
    template <class Y, geometry::point_cloud_c cloudT>
    constexpr auto point_field(const cloudT & cloud, std::string name)
    {
        // build a point field on the points collected from the cloud
        return point_field_t<cloudT::dim, Y>(cloud.points(), name);
    }

    // discrete system factory
    template <class functionSpaceT>
    constexpr auto discrete_system(const functionSpaceT & function_space)
    {
        return discrete_system_t<functionSpaceT>(function_space);
    }
}


// end of file
