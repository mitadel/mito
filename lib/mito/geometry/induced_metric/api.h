// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // Factory: induced metric from Jacobian field and parametrization
    template <int N, coordinates_c coordsT, class JacobianT, class ParamT>
    constexpr auto induced_metric(JacobianT && jacobian, ParamT && parametrization);

}    // namespace mito::geometry


// end of file
