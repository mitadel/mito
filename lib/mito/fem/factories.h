// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

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
        return discrete::nodal_field_t<Y>(nodes, name);
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
