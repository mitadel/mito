// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::petsc {

    // petsc linear system
    using linear_system_t = PETScLinearSystem;

    // petsc Krlov solver
    using ksp_t = PETScKrylovSolver;

}


// end of file
