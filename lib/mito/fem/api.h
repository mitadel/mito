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
    template <class lhsBlockT, class rhsBlockT>
    using weakform_t = Weakform<lhsBlockT, rhsBlockT>;

    // weakform factory
    template <class lhsBlockT, class rhsBlockT>
    constexpr auto weakform(const lhsBlockT & lhs_block, const rhsBlockT & rhs_block);

    // discrete system alias
    template <class functionSpaceT, class weakformT, class linearSystemT>
    using discrete_system_t = DiscreteSystem<functionSpaceT, weakformT, linearSystemT>;

    // discrete system factory
    template <class linearSystemT, class functionSpaceT, class weakformT>
    constexpr auto discrete_system(
        const functionSpaceT & function_space, const weakformT & weakform,
        const std::string & label);
}


// end of file
