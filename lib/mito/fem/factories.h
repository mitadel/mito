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

    // discrete system factory (variadic: takes Contribution objects)
    template <class linearSystemT, function_space_c... functionSpaceTs>
    constexpr auto discrete_system(
        const std::string & label, Contribution<functionSpaceTs>... contributions)
    {
        return discrete_system_t<linearSystemT, functionSpaceTs...>(label, contributions...);
    }

    // discrete system factory (single space: convenience overload)
    template <class linearSystemT, function_space_c functionSpaceT>
    constexpr auto discrete_system(
        const std::string & label, const functionSpaceT & function_space,
        const weakform_t<typename functionSpaceT::element_type> & weakform)
    {
        return discrete_system_t<linearSystemT, functionSpaceT>(
            label, Contribution<functionSpaceT>{ function_space, weakform, 1.0 });
    }
}


// end of file
