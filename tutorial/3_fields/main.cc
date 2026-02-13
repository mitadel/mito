// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// mito support
#include <mito.h>


// strip namespace
using namespace mito;
using namespace mito::fields;
using namespace mito::tensor;

// the type of coordinates
using coordinates_t = geometry::cartesian::coordinates_t<2>;


int
main()
{
    // make a channel
    journal::info_t channel("tutorial.fields");

    // report
    channel << "a mito application" << journal::endl;

    // function sin
    auto sin = functions::sin;

    // function cos
    auto cos = functions::cos;

    // x_0 in cartesian coordinates
    auto x_0 = geometry::cartesian::x_0<2>;

    // x_1 in cartesian coordinates
    auto x_1 = geometry::cartesian::x_1<2>;

    // a vector field
    auto f = sin(x_0 * x_1) * e_0<2> + cos(x_0 * x_1) * e_1<2>;

    // a point in space
    auto x = geometry::cartesian::coordinates({ 1.0, -0.5 });

    // the divergence of the gradient transposed of {f}
    auto div_grad_T = divergence(transpose(gradient(f)));

    // the gradient of the divergence of {f}
    auto grad_div = gradient(divergence(f));

    // report result
    channel << "div(grad^T(f)) = " << div_grad_T(x) << journal::endl;
    channel << "grad(div(f)) = " << grad_div(x) << journal::endl;

    // all done
    return 0;
}