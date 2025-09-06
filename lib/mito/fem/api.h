// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // nodal field factory
    template <class Y, function_space_c functionSpaceT>
    constexpr auto nodal_field(const functionSpaceT & function_space, std::string name);

    // function space alias
    template <class elementT, constraints::constraint_c constraintsT>
    using function_space_t = FunctionSpace<elementT, constraintsT>;

    // function space factory
    template <manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    constexpr auto function_space(const manifoldT & manifold, const constraintsT & constraints);

    // discrete system alias
    template <class functionSpaceT, class linearSystemT>
    using discrete_system_t = DiscreteSystem<functionSpaceT, linearSystemT>;

    // discrete system factory
    template <class linearSystemT, class functionSpaceT>
    constexpr auto discrete_system(
        const functionSpaceT & function_space, const std::string & label);
}


// end of file
