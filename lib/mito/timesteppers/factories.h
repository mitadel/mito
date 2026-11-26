// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // implicit euler factory
    template <
        linalg::valid_backend_c linalgBackendT, template <class, class, class> class stageSolverT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    constexpr auto implicit_euler(
        const problemT & problem, solverT & solver,
        const typename linalgBackendT::vector_type & initial_condition,
        typename linalgBackendT::scalar_type dt)
        -> implicit_euler_t<linalgBackendT, problemT, solverT, stageSolverT>
    {
        return implicit_euler_t<linalgBackendT, problemT, solverT, stageSolverT>(
            problem, solver, initial_condition, dt);
    }

    // crank-nicholson factory
    template <
        linalg::valid_backend_c linalgBackendT, template <class, class, class> class stageSolverT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    constexpr auto crank_nicholson(
        const problemT & problem, solverT & solver,
        const typename linalgBackendT::vector_type & initial_condition,
        typename linalgBackendT::scalar_type dt)
        -> crank_nicholson_t<linalgBackendT, problemT, solverT, stageSolverT>
    {
        return crank_nicholson_t<linalgBackendT, problemT, solverT, stageSolverT>(
            problem, solver, initial_condition, dt);
    }

    // sdirk3s3o factory
    template <
        linalg::valid_backend_c linalgBackendT, template <class, class, class> class stageSolverT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    constexpr auto sdirk3s3o(
        const problemT & problem, solverT & solver,
        const typename linalgBackendT::vector_type & initial_condition,
        typename linalgBackendT::scalar_type dt)
        -> sdirk3s3o_t<linalgBackendT, problemT, solverT, stageSolverT>
    {
        return sdirk3s3o_t<linalgBackendT, problemT, solverT, stageSolverT>(
            problem, solver, initial_condition, dt);
    }
}


// end of file
