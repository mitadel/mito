// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::time_steppers {

    template <
        class discreteSystemT, class matrixSolverT,
        mito::math_backend::valid_backend_c mathBackendT>
    class ExplicitEuler;
}


// end of file
