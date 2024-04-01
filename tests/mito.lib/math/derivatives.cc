// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/math.h>


namespace mito::math {

    constexpr auto cos =
        function([](const mito::vector_t<1> & x) -> mito::scalar_t { return std::cos(x); });
    constexpr auto sin =
        function([](const mito::vector_t<1> & x) -> mito::scalar_t { return std::sin(x); });
    constexpr auto x_0 =
        function([](const mito::vector_t<2> & x) -> mito::scalar_t { return x[0]; });
    constexpr auto x_1 =
        function([](const mito::vector_t<2> & x) -> mito::scalar_t { return x[1]; });
}


// typedef
using scalar_function_type = mito::scalar_t (*)(const mito::vector_t<1> &);


int
main()
{

    constexpr auto f = mito::math::cos(mito::math::x_0 * mito::math::x_1);

    std::cout << f({ 0.0, 1.0 }) << std::endl;
    std::map<scalar_function_type, scalar_function_type> derivatives;
    derivatives[mito::math::sin] = mito::math::cos;

    std::cout << derivatives[mito::math::sin]({ std::numbers::pi }) << std::endl;

    // all done
    return 0;
}