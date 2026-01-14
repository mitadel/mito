// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// first order shape functions type
using shape_t = mito::fem::ShapeSegmentP1;
// the parametric coordinates type
using parametric_coordinates_type = shape_t::reference_element_type::parametric_coordinates_type;


TEST(Fem, ShapeSegmentP1)
{
    // first order shape functions
    constexpr auto element = shape_t();

    // node 0 in parametric coordinates
    constexpr auto n0 = parametric_coordinates_type{ 0.0 };
    // node 1 in parametric coordinates
    constexpr auto n1 = parametric_coordinates_type{ 1.0 };

    // the shape function associated with local node {0}
    constexpr auto phi_0 = element.shape<0>();
    // check that the shape function at node 0 is 1.0
    static_assert(1.0 == phi_0(n0));
    // check that the shape function at node 1 is 0.0
    static_assert(0.0 == phi_0(n1));

    // the shape functions at node 1
    constexpr auto phi_1 = element.shape<1>();
    // check that the shape function at node 0 is 0.0
    static_assert(0.0 == phi_1(n0));
    // check that the shape function at node 1 is 1.0
    static_assert(1.0 == phi_1(n1));

    // all done
    return;
}
