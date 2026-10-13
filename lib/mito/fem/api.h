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

    // weakform term alias
    template <class sourceT, class blockT>
    using term_t = Term<sourceT, blockT>;

    // weakform term factory
    template <class sourceT, class blockT>
    constexpr auto term(const sourceT & source, const blockT & block);

    // coupled weakform alias
    template <class... termTs>
    using coupled_weakform_t = CoupledWeakform<termTs...>;

    // coupled weakform factory
    template <class... termTs>
    constexpr auto coupled_weakform(const termTs &... terms);

    // discrete system alias
    template <class linearSystemT, class coupledWeakformT>
    using discrete_system_t = DiscreteSystem<linearSystemT, coupledWeakformT>;

    // discrete system factory
    template <class linearSystemT, class coupledWeakformT>
    constexpr auto discrete_system(const std::string & label, const coupledWeakformT & weakform);
}


// end of file
