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

    // nodal field factory
    template <class Y, function_space_c functionSpaceT>
    constexpr auto nodal_field(const functionSpaceT & function_space, std::string name)
    {
        // assemble the node type
        using node_type = functionSpaceT::discretization_node_type;

        // get the nodes in the mesh
        std::unordered_set<node_type, utilities::hash_function<node_type>> nodes;
        get_discretization_nodes(function_space, nodes);

        // build a nodal field on the discretization nodes collected from the function space
        return nodal_field_t<Y>(nodes, name);
    }

    // TOFIX: create a constructor that takes no constraints

    // TOFIX: {constraints} should be a collection of constraints as opposed to an instance of a
    // single constraint
    // function space factory
    template <int p, manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    constexpr auto function_space(const manifoldT & manifold, const constraintsT & constraints)
    {
        // the manifold type
        using manifold_type = manifoldT;
        // the mesh type
        using mesh_type = typename manifold_type::mesh_type;
        // the cell type
        using cell_type = typename mesh_type::cell_type;
        // the degree of the finite element
        constexpr int degree = p;
        // typedef for a finite element
        using element_type = typename isoparametric_simplex<degree, cell_type>::type;

        // build a function space on the manifold and return it
        return function_space_t<element_type, constraintsT>(manifold, constraints);
    }

    // discrete system factory
    template <class linearSystemT, class functionSpaceT>
    constexpr auto discrete_system(const functionSpaceT & function_space, const std::string & label)
    {
        return discrete_system_t<functionSpaceT, linearSystemT>(function_space, label);
    }
}


// end of file
