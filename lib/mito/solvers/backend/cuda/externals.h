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


// cuda support
#include <cuda_runtime.h>
#include <cusolverDn.h>


namespace mito::solvers::cuda {

    // concept for a real type
    template <typename T>
    concept real_c = std::is_floating_point_v<T>;

}


// end of file
