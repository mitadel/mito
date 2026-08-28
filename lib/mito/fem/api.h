// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // finite element field alias
    template <class fieldValueT>
    using fem_field_t = FemField<fieldValueT>;

    // the possible discretization types: continuous Galerking (CG) vs. discontinuous Galerkin (DG)
    enum class discretization_t { CG, DG };

    // function space alias
    template <class elementT, class manifoldT, constraints::constraint_c constraintsT>
    using function_space_t = FunctionSpace<elementT, manifoldT, constraintsT>;

    // function space factory
    template <
        class elementT, manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    // require compatibility between the manifold cell and the finite element cell
    requires(
        std::is_same_v<typename manifoldT::mesh_type::cell_type, typename elementT::mesh_cell_type>)
    constexpr auto function_space(const manifoldT & manifold, const constraintsT & constraints);

    // function space elements view alias
    template <class functionSpaceT>
    using function_space_elements_view_t = FunctionSpaceElementsView<functionSpaceT>;

    // weakform alias
    template <class lhsBlockT, class rhsBlockT>
    using weakform_t = Weakform<lhsBlockT, rhsBlockT>;

    // weakform factory
    template <class lhsBlockT, class rhsBlockT>
    constexpr auto weakform(const lhsBlockT & lhs_block, const rhsBlockT & rhs_block);

    // discrete system alias
    template <class linearSystemT, contribution_c... contributionTs>
    using discrete_system_t = DiscreteSystem<linearSystemT, contributionTs...>;

    // discrete system factory (one contribution per function space)
    template <class linearSystemT, contribution_c... contributionTs>
    constexpr auto discrete_system(
        const std::string & label, const contributionTs &... contributions);

    // discrete system factory (single function space)
    template <class linearSystemT, function_space_c functionSpaceT, class weakformT>
    constexpr auto discrete_system(
        const std::string & label, const functionSpaceT & function_space,
        const weakformT & weakform);
}


// end of file
