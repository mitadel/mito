// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// externals
#define HAVE_TENSOR
#define HAVE_COMPACT_PACKINGS
#include <pyre/tensor.h>


// get the {pyre::tensor} sequences of integers
template <int N>
using make_integer_sequence = pyre::tensor::make_integer_sequence<N>;
template <int... I>
using integer_sequence = pyre::tensor::integer_sequence<I...>;


// end of file
