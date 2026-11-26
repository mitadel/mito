// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the linear algebra backend type
using linalg_backend_t = mito::linalg::petsc::backend_t;
// the vector type
using vector_t = typename linalg_backend_t::vector_type;
// the solver type
using matrix_solver_t = mito::linalg::petsc::ksp_t;


// the initial condition
constexpr mito::real u0_0 = 0.0;
constexpr mito::real u0_1 = 1.5;

// the final time
constexpr mito::real final_time = 1.0;

// the ODE problem type, M du/dt + K u = f, with M, K, f constant in time
// M = diag(m0, m1), K = diag(k0, k1), f = (f0, f1)
struct ode_problem_t {
    // the number of equations
    static constexpr int n_equations() { return 2; }
    static constexpr mito::real m0 = 2.0;
    static constexpr mito::real m1 = 4.0;
    static constexpr mito::real k0 = 1.0;
    static constexpr mito::real k1 = 3.0;
    static constexpr mito::real f0 = 1.0;
    static constexpr mito::real f1 = 2.0;

    // set the sparsity pattern of a system matrix
    template <mito::linalg::valid_backend_c backendT>
    auto set_pattern(typename backendT::matrix_type & A) const -> void
    {
        A.set_pattern({ 0, 0, 1, 1 }, { 0, 1, 0, 1 });
    }

    // assemble the mass matrix
    template <mito::linalg::valid_backend_c backendT>
    auto assemble_mass(typename backendT::matrix_type & A) const -> void
    {
        A.set_values({ m0, 0.0, 0.0, m1 });
        A.assemble();
    }

    // assemble the stiffness matrix
    template <mito::linalg::valid_backend_c backendT>
    auto assemble_stiffness(typename backendT::matrix_type & A) const -> void
    {
        A.set_values({ k0, 0.0, 0.0, k1 });
        A.assemble();
    }

    // assemble the load vector
    template <mito::linalg::valid_backend_c backendT>
    auto assemble_load(typename backendT::vector_type & v) const -> void
    {
        v.insert_value(0, f0);
        v.insert_value(1, f1);
        v.assemble();
    }

    // the exact solution at time {t} starting from (u0_0, u0_1), written into {u}
    auto exact_solution(mito::real t, vector_t & u) const -> void
    {
        // steady-state
        auto s0 = f0 / k0;
        auto s1 = f1 / k1;

        // transient
        auto t0 = (u0_0 - s0) * std::exp(-k0 * t / m0);
        auto t1 = (u0_1 - s1) * std::exp(-k1 * t / m1);

        u.insert_value(0, s0 + t0);
        u.insert_value(1, s1 + t1);
        u.assemble();
    }

    // the problem is time invariant
    static constexpr bool time_invariant = true;

    // the problem has a constant load
    static constexpr bool constant_load = true;
};

// the timestepper type
using timestepper_t = mito::timesteppers::sdirk3s3o_t<
    linalg_backend_t, ode_problem_t, matrix_solver_t, mito::timesteppers::LinearStageSolver>;

auto
error(mito::real dt) -> mito::real
{
    // the problem
    const auto problem = ode_problem_t();

    // the matrix solver
    auto solver = matrix_solver_t("linear_stage_solver");

    // the initial condition
    auto u_0 = vector_t("initial_condition", problem.n_equations());
    u_0.insert_value(0, u0_0);
    u_0.insert_value(1, u0_1);
    u_0.assemble();

    // the linear stage solver
    auto time_stepper = timestepper_t(problem, solver, u_0, dt);

    // march to the final time
    auto n_steps = static_cast<int>(std::round(final_time / dt));
    for (auto step = 0; step < n_steps; ++step) {
        time_stepper.march();
    }

    // the exact solution
    auto u_exact = vector_t("solution_exact", problem.n_equations());
    problem.exact_solution(final_time, u_exact);

    // the error
    auto u = time_stepper.solution();
    u.add(-1.0, u_exact);
    return u.norm();
}


TEST(Timesteppers, SDIRK3S3O_ODE_CONVERGENCE)
{
    // initialise PETSc environment
    auto petsc_env = mito::linalg::petsc::environment();

    // the time step
    constexpr mito::real dt_coarse = 0.1;
    constexpr mito::real dt_fine = dt_coarse / 2.0;

    // the error at the coarse and fine time steps
    auto e_coarse = error(dt_coarse);
    auto e_fine = error(dt_fine);

    // the observed order of accuracy
    auto p = std::log(e_coarse / e_fine) / std::log(dt_coarse / dt_fine);

    // check the order of accuracy
    EXPECT_NEAR(p, 3.0, 0.1);

    // all done
    return;
}