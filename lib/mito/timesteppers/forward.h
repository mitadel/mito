// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    template <
        linalg::valid_backend_c linalgBackendT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    class LinearStageSolver;

    template <
        linalg::valid_backend_c linalgBackendT, class problemT, class solverT,
        template <class, class, class> class stageSolverT>
    class ImplicitEuler;

    template <
        linalg::valid_backend_c linalgBackendT, class problemT, class solverT,
        template <class, class, class> class stageSolverT>
    class CrankNicholson;

    template <
        linalg::valid_backend_c linalgBackendT, class problemT, class solverT,
        template <class, class, class> class stageSolverT>
    class SDIRK3S3O;
}


// end of file
