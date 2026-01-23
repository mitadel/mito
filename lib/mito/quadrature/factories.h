// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::quadrature {

    // integrator factory
    template <quadrature_formula quadratureT, int r, class manifoldT>
    constexpr auto integrator(const manifoldT & manifold) -> integrator_t<quadratureT, r, manifoldT>
    {
        return integrator_t<quadratureT, r, manifoldT>(manifold);
    }

    template <quadrature_formula quadratureT, class elementT, int r>
    constexpr auto quadrature_rule() -> quadrature_rule_t<quadratureT, elementT, r>
    {
        return quadrature_rule_t<quadratureT, elementT, r>();
    }
}


// end of file
