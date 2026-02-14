// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// externals
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <type_traits>
#ifdef WITH_EIGEN
#include <Eigen/Sparse>    // for sparse matrix support
#endif                     // WITH_EIGEN


// cuda support
#include <cuda_runtime.h>
#include <cusolverDn.h>
#ifdef WITH_EIGEN
#include <cudss.h>
#endif    // WITH_EIGEN


// utilities
#include "utilities.h"


namespace mito::solvers::cuda {

    // concept for a real type
    template <typename T>
    concept real_c = std::is_floating_point_v<T>;

}


// end of file
