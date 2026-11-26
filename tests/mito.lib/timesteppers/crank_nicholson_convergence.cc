// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include "utilities/convergence.h"


using namespace mito::tests::timesteppers;


TEST(Timesteppers, Crank_Nicholson_Convergence)
{
    // initialise PETSc environment
    auto petsc_env = mito::linalg::petsc::environment();

    // the observed order of accuracy of crank-Nicholson
    const auto p = observed_order(
        [](const auto & problem, auto & solver, const auto & initial_condition, scalar_t dt) {
            return mito::timesteppers::crank_nicholson<linalg_backend_t, stage_solver_t>(
                problem, solver, initial_condition, dt);
        });

    // report
    journal::info_t channel("tests.timesteppers.crank_nicholson_convergence");
    channel << "observed order of accuracy: " << p << journal::endl;

    // crank-Nicholson is second order
    EXPECT_NEAR(p, 2.0, 0.1);

    // all done
    return;
}


// end of file