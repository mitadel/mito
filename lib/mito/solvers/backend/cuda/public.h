// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// external packages
#include "externals.h"

// get the forward declarations
#include "forward.h"

// published types
#include "api.h"

// classes
#include "CUDASolver.h"
#include "CUDADenseSolver.h"
#ifdef WITH_EIGEN
#include "CUDASparseSolver.h"
#endif    // WITH_EIGEN

// error checks
#include "CUDAErrorChecks.h"

// factories implementation
#include "factories.h"


// end of file
