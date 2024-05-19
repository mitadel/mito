// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::quadrature {

    // available quadrature formulae
    enum quadrature_formula { GAUSS };

    // class integrator
    template <quadrature_formula quadratureT, int r, class manifoldT>
    class Integrator;

    // class quadrature rule
    template <quadrature_formula quadratureT, class elementT, int r>
    class QuadratureRule;
}


// end of file
