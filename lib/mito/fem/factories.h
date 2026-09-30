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

    // weakform term factory
    template <class sourceT, class blockT>
    constexpr auto term(const sourceT & source, const blockT & block)
    {
        return term_t<sourceT, blockT>{ source, block };
    }

    // coupled weakform factory
    template <class... termTs>
    constexpr auto coupled_weakform(const termTs &... terms)
    {
        return coupled_weakform_t<termTs...>(terms...);
    }

    // discrete system factory
    template <class linearSystemT, class coupledWeakformT>
    constexpr auto discrete_system(const std::string & label, const coupledWeakformT & weakform)
    {
        return discrete_system_t<linearSystemT, coupledWeakformT>(label, weakform);
    }
}


// end of file
