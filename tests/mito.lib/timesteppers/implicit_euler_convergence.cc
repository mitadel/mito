// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include "utilities/convergence.h"


using namespace mito::tests::timesteppers;


TEST(Timesteppers, IMPLICIT_EULER_CONVERGENCE)
{
    // initialise PETSc environment
    auto petsc_env = mito::linalg::petsc::environment();

    // the observed order of accuracy of implicit Euler
    const auto p = observed_order(
        [](const auto & model, auto && solver, const auto & initial_condition, scalar_t dt) {
            return mito::timesteppers::implicit_euler<linalg_backend_t>(
                model, std::move(solver), initial_condition, dt);
        });

    // report
    journal::info_t channel("tests.timesteppers.implicit_euler_convergence");
    channel << "observed order of accuracy: " << p << journal::endl;

    // implicit Euler is first order
    EXPECT_NEAR(p, 1.0, 0.1);

    // all done
    return;
}


// end of file