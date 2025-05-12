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

    // TOFIX: {constraints} should be a collection of constraints as opposed to an instance of a
    // single constraint
    // function space factory
    template <int p, manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    constexpr auto function_space(const manifoldT & manifold, const constraintsT & constraints)
    {
        // build a function space on the manifold and return it
        return function_space_t<p, manifoldT, constraintsT>(manifold, constraints);
    }

    // discrete system factory
    template <class functionSpaceT>
    constexpr auto discrete_system(const functionSpaceT & function_space)
    {
        return discrete_system_t<functionSpaceT>(function_space);
    }
}


// end of file
