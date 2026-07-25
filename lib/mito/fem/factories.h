// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // TOFIX: create a constructor that takes no constraints

    // TOFIX: {constraints} should be a collection of constraints as opposed to an instance of a
    // single constraint
    // function space factory
    template <
        class elementT, manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    // require compatibility between the manifold cell and the finite element cell
    requires(
        std::is_same_v<typename manifoldT::mesh_type::cell_type, typename elementT::mesh_cell_type>)
    constexpr auto function_space(const manifoldT & manifold, const constraintsT & constraints)
    {
        // build a function space on the manifold and return it
        return function_space_t<elementT, manifoldT, constraintsT>(manifold, constraints);
    }

    // function space factory with a pre-populated node map (for coupled problems that share
    // discretization nodes with another function space)
    template <
        class elementT, manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    // require compatibility between the manifold cell and the finite element cell
    requires(
        std::is_same_v<typename manifoldT::mesh_type::cell_type, typename elementT::mesh_cell_type>)
    constexpr auto function_space(
        const manifoldT & manifold, const constraintsT & constraints,
        const typename function_space_t<elementT, manifoldT, constraintsT>::map_type &
            shared_node_map)
    {
        // build a function space on the manifold, reusing the discretization nodes of the mesh
        // nodes already present in {shared_node_map}
        return function_space_t<elementT, manifoldT, constraintsT>(
            manifold, constraints, shared_node_map);
    }

    // weakform factory
    template <class lhsBlockT, class rhsBlockT>
    constexpr auto weakform(const lhsBlockT & lhs_block, const rhsBlockT & rhs_block)
    {
        return weakform_t<lhsBlockT, rhsBlockT>(lhs_block, rhs_block);
    }

    // discrete system factory (one contribution per function space)
    template <class linearSystemT, contribution_c... contributionTs>
    constexpr auto discrete_system(
        const std::string & label, const contributionTs &... contributions)
    {
        return discrete_system_t<linearSystemT, contributionTs...>(label, contributions...);
    }

    // discrete system factory (single function space)
    template <class linearSystemT, function_space_c functionSpaceT, class weakformT>
    constexpr auto discrete_system(
        const std::string & label, const functionSpaceT & function_space,
        const weakformT & weakform)
    {
        return discrete_system_t<linearSystemT, Contribution<functionSpaceT, weakformT>>(
            label, Contribution<functionSpaceT, weakformT>{ function_space, weakform });
    }
}


// end of file
