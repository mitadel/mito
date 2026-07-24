// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // finite element field alias
    template <class fieldValueT, class functionSpaceT>
    using fem_field_t = FemField<fieldValueT, functionSpaceT>;

    // the possible discretization types: continuous Galerking (CG) vs. discontinuous Galerkin (DG)
    enum class discretization_t { CG, DG };

    // function space alias
    template <class elementT, constraints::constraint_c constraintsT>
    using function_space_t = FunctionSpace<elementT, constraintsT>;

    // function space factory
    template <manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    constexpr auto function_space(const manifoldT & manifold, const constraintsT & constraints);

    // weakform alias
    template <class finiteElementT>
    using weakform_t = Weakform<finiteElementT>;

    // weakform factory
    template <class finiteElementT>
    constexpr auto weakform();

    // discrete system alias
    template <class linearSystemT, function_space_c... functionSpaceTs>
    using discrete_system_t = DiscreteSystem<linearSystemT, functionSpaceTs...>;

    // discrete system factory (variadic: takes Contribution objects)
    template <class linearSystemT, function_space_c... functionSpaceTs>
    constexpr auto discrete_system(
        const std::string & label, Contribution<functionSpaceTs>... contributions);

    // discrete system factory (single space: convenience overload)
    template <class linearSystemT, function_space_c functionSpaceT>
    constexpr auto discrete_system(
        const std::string & label, const functionSpaceT & function_space,
        const weakform_t<typename functionSpaceT::element_type> & weakform);
}


// end of file
