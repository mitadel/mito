// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // class discretization node
    class DiscretizationNode;

    // class discrete field
    template <class keyT, class Y>
    class DiscreteField;

    // class function space
    template <int p, manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    class FunctionSpace;

    // class discrete system
    template <class functionSpaceT>
    class DiscreteSystem;
}


// end of file
