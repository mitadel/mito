// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // discretization node alias
    using discretization_node_t = utilities::std_shared_ptr<const DiscretizationNode>;

    // function space alias
    template <int p, manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    using function_space_t = FunctionSpace<p, manifoldT, constraintsT>;

    // function space factory
    template <manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    constexpr auto function_space(const manifoldT & manifold, const constraintsT & constraints);

}


// end of file
