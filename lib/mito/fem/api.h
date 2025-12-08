// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // nodal field factory
    template <class Y, function_space_c functionSpaceT>
    constexpr auto nodal_field(const functionSpaceT & function_space, std::string name);

    // domain field alias
    template <fields::field_c F>
    using domain_field_t = DomainField<F>;

    // domain field factory
    template <fields::field_c F>
    constexpr auto domain_field(const F &);

    // finite element field alias
    template <class fieldValueT>
    using fem_field_t = FemField<fieldValueT>;

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
    template <class functionSpaceT, class linearSystemT>
    using discrete_system_t = DiscreteSystem<functionSpaceT, linearSystemT>;

    // discrete system factory
    template <class linearSystemT, class functionSpaceT>
    constexpr auto discrete_system(
        const functionSpaceT & function_space, const std::string & label);
}


// end of file
