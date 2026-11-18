// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // implicit euler alias
    template <
        linalg::valid_backend_c linalgBackendT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    using implicit_euler_t = ImplicitEuler<linalgBackendT, problemT, solverT>;

    // implicit euler factory
    template <
        linalg::valid_backend_c linalgBackendT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    constexpr auto implicit_euler(
        const problemT & problem, solverT && solver, const fields::field_c auto & initial_condition,
        typename linalgBackendT::scalar_type dt)
        -> implicit_euler_t<linalgBackendT, problemT, solverT>;

    // crank-nicholson alias
    template <
        linalg::valid_backend_c linalgBackendT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    using crank_nicholson_t = CrankNicholson<linalgBackendT, problemT, solverT>;

    // crank-nicholson factory
    template <
        linalg::valid_backend_c linalgBackendT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    constexpr auto crank_nicholson(
        const problemT & problem, solverT && solver, const fields::field_c auto & initial_condition,
        typename linalgBackendT::scalar_type dt)
        -> crank_nicholson_t<linalgBackendT, problemT, solverT>;

    // sdirk3s3o alias
    template <
        linalg::valid_backend_c linalgBackendT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    using sdirk3s3o_t = SDIRK3S3O<linalgBackendT, problemT, solverT>;

    // sdirk3s3o factory
    template <
        linalg::valid_backend_c linalgBackendT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    constexpr auto sdirk3s3o(
        const problemT & problem, solverT && solver, const fields::field_c auto & initial_condition,
        typename linalgBackendT::scalar_type dt) -> sdirk3s3o_t<linalgBackendT, problemT, solverT>;
}


// end of file
