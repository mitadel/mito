// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // function space alias
    template <class elementT, constraints::constraint_c constraintsT>
    using function_space_t = FunctionSpace<elementT, constraintsT>;

    // function space factory
    template <manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    constexpr auto function_space(const manifoldT & manifold, const constraintsT & constraints);

}


// end of file
