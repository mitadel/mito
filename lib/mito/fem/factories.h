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

    // domain field factory
    template <fields::field_c F>
    constexpr auto domain_field(const F & field)
    {
        // build a domain field from the given function
        return domain_field_t<F>(field);
    }

    // TOFIX: create a constructor that takes no constraints

    // TOFIX: {constraints} should be a collection of constraints as opposed to an instance of a
    // single constraint
    // function space factory
    template <
        class elementT, manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    // require compatibility between the manifold cell and the finite element cell
    requires(std::is_same_v<typename manifoldT::mesh_type::cell_type, typename elementT::cell_type>)
    constexpr auto function_space(const manifoldT & manifold, const constraintsT & constraints)
    {
        // build a function space on the manifold and return it
        return function_space_t<elementT, constraintsT>(manifold, constraints);
    }

    // weakform factory
    template <class finiteElementT>
    constexpr auto weakform()
    {
        return weakform_t<finiteElementT>();
    }

    // discrete system factory
    template <class linearSystemT, class functionSpaceT, class weakformT>
    constexpr auto discrete_system(
        const std::string & label, const functionSpaceT & function_space,
        const weakformT & weakform)
    {
        return discrete_system_t<functionSpaceT, linearSystemT>(label, function_space, weakform);
    }
}


// end of file
