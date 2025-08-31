// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/functions.h>


// pi
using std::numbers::pi;


TEST(Derivatives, Subscript)
{
    // a scalar
    constexpr auto x = pi / 4.0;

    // a vector function R -> R^2x3
    constexpr auto f = mito::functions::sin * mito::tensor::e_12<2, 3>;

    // get the output type of the function {f}, in this case a 2x3 tensor
    using output_type = decltype(f)::output_type;

    // get the offset corresponding to the multi-index {1, 2} in a 2x3 tensor
    // multi-index {1, 2} is where {f} has the only nontrivial value
    constexpr int offset = output_type::getOffset<1, 2>();

    // check that evaluating and subscripting is the same than subscripting and evaluating
    static_assert(f(x)[offset] == f[offset](x));

    // check that evaluating and subscripting is the same than subscripting and evaluating
    static_assert(f(x)[{ 1, 2 }] == f[{ 1, 2 }](x));

    // check that the derivative of the subscript is the subscript of the derivative
    static_assert(
        mito::functions::derivative(f[offset])(x) == mito::functions::derivative(f)(x)[offset]);

    // check that the derivative of the scalar function which extracts the {1, 2} component is equal
    // to the {{1, 2}, 0} component of the total derivative
    static_assert(
        mito::functions::derivative(f[{ 1, 2 }])(x)
        == mito::functions::derivative(f)(x)[{ offset, 0 }]);
}


// end of file
