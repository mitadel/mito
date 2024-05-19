// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fem.h>


template <int Q, class Y>
void
constFunction(const mito::fem::quadrature_field_t<Q, Y> & quadF)
{
    // get a (deep) copy of the vector at (0, 1)
    auto vector = quadF(0, 1);

    // edit the copy
    vector[1] = 10;

    // all done
    return;
}

TEST(Fem, QuadratureFields)
{
    // say we have 20 elements
    int nElements = 20;
    // and 10 quadrature points
    constexpr int Q = 10;

    // a field of {Q} mito vectors
    auto field = mito::fem::quadrature_field_t<Q, mito::vector_t<2>>(nElements);

    // get a reference the (0, 1) vector
    auto & vector = field(0, 1);

    // expect that the vector size is 2
    EXPECT_TRUE(vector.size == 2);

    // modify the content of {vector}
    vector[0] = 0.0;
    vector[1] = 1.0;

    // expect that editing {vector} has edited the {field}
    EXPECT_DOUBLE_EQ(0.0, field(0, 1)[0]);
    EXPECT_DOUBLE_EQ(1.0, field(0, 1)[1]);

    // call a function that modifies a (deep) copy of the field
    constFunction(field);

    // expect that the {field} is unchanged (deep copy vs. shallow copy)
    EXPECT_DOUBLE_EQ(0.0, field(0, 1)[0]);
    EXPECT_DOUBLE_EQ(1.0, field(0, 1)[1]);
}