// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // class discrete field
    template <class keyT, class Y>
    class DiscreteField;

    // class isoparametric simplex
    template <geometry::geometric_simplex_c geometricSimplexT>
    class IsoparametricSimplex;

    // class function space
    template <manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    class FunctionSpace;

    // class discrete system
    template <class functionSpaceT>
    class DiscreteSystem;
}


// end of file
