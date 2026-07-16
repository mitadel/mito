// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// mito support
#include <mito.h>


// strip namespace
using namespace mito::tensor;


int
main()
{
    // make a channel
    journal::info_t channel("tutorial.tensors");

    // report
    channel << "a mito application" << journal::endl;

    {
        // a 2D vector
        auto a = vector_t<2>{ 1.0, 1.0 };
        // a 2x2 matrix
        auto B = matrix_t<2, 2>{ 2.0, 0.0, 1.0, 2.0 };
        // a quadratic form
        auto q = transpose(a) * B * a;
        // report result
        channel << "a^T * B * a = " << q << journal::endl;
    }

    {
        // a 2D vector
        auto a = e_0<2> + e_1<2>;
        // a 2x2 matrix
        auto B = 2.0 * e_00<2> + 0.0 * e_01<2> + 1.0 * e_10<2> + 2.0 * e_11<2>;
        // a quadratic form
        auto q = transpose(a) * B * a;
        // report result
        channel << "a^T * B * a = " << q << journal::endl;
    }

    // all done
    return 0;
}