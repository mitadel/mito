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

    // the problem is time invariant
    static constexpr bool time_invariant = true;

    // the problem has a constant load
    static constexpr bool constant_load = true;
};

// the linear stage solver type
using linear_stage_solver_t =
    mito::timesteppers::LinearStageSolver<linalg_backend_t, ode_problem_t, matrix_solver_t>;


TEST(Timesteppers, IMPLICIT_EULER_SET_DT)
{
    // initialise PETSc environment
    auto petsc_env = mito::linalg::petsc::environment();

    // the time step
    constexpr mito::real dt = 0.1;

    // the beta vector
    constexpr mito::real beta0 = 1.0;
    constexpr mito::real beta1 = 2.0;

    // the problem
    const auto problem = ode_problem_t();

    // the matrix solver
    auto solver = matrix_solver_t("linear_stage_solver");

    // the linear stage solver
    auto linear_stage_solver = linear_stage_solver_t(problem, solver, dt);

    // the beta vector
    auto beta = vector_t("rhs", problem.n_equations());
    beta.insert_value(0, beta0);
    beta.insert_value(1, beta1);
    beta.assemble();

    // the solution
    auto u = vector_t("solution", problem.n_equations());

    // solve the linear system, alpha * M * (u - beta) - (f - K * u) = 0
    linear_stage_solver.solve(dt, beta, u);

    // the closed form solution, u_i = (f_i + alpha * M_i * beta_i) / (K_i + alpha * M_i)
    auto u_exact = vector_t("solution_exact", problem.n_equations());
    const auto u0 = (ode_problem_t::f0 + dt * ode_problem_t::m0 * beta0)
                  / (ode_problem_t::k0 + dt * ode_problem_t::m0);
    const auto u1 = (ode_problem_t::f1 + dt * ode_problem_t::m1 * beta1)
                  / (ode_problem_t::k1 + dt * ode_problem_t::m1);
    u_exact.insert_value(0, u0);
    u_exact.insert_value(1, u1);
    u_exact.assemble();

    // check the solution
    u.add(-1.0, u_exact);
    const auto error = u.norm();
    EXPECT_NEAR(error, 0.0, 1e-12);

    // all done
    return;
}