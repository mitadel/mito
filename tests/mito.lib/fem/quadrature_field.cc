// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fem.h>


// pick a cell type
using cell_type = mito::topology::oriented_simplex_t<1>;
// one quadrature points per cell
constexpr int Q = 1;
// a type for the field
using vector_type = mito::tensor::vector_t<2>;
// assemble the quadrature field
using quadrature_field_type = mito::fem::quadrature_field_t<cell_type, Q, vector_type>;

// an empty topology
auto & topology = mito::topology::topology();

// build a segment
const auto segment0 = topology.segment({ topology.vertex(), topology.vertex() });


void
constFunction(const quadrature_field_type & field)
{
    // get a (deep) copy of the vector at (0)
    auto vector = field(segment0)[0];

    // edit the copy
    vector[1] = 10;

    // all done
    return;
}

TEST(Fem, QuadratureFields)
{
    // a quadrature field
    auto field = quadrature_field_type("field");

    // insert one entry using the default value
    field.insert(segment0);

    // get a reference the 0-th vector in the field
    auto & vector = field(segment0)[0];

    // expect that the vector size is 2
    EXPECT_TRUE(vector.size == 2);

    // modify the content of {vector}
    vector[0] = 0.0;
    vector[1] = 1.0;

    // expect that editing {vector} has edited the {field}
    EXPECT_DOUBLE_EQ(0.0, field(segment0)[0][0]);
    EXPECT_DOUBLE_EQ(1.0, field(segment0)[0][1]);

    // call a function that modifies a (deep) copy of the field
    constFunction(field);

    // expect that the {field} is unchanged (deep copy vs. shallow copy)
    EXPECT_DOUBLE_EQ(0.0, field(segment0)[0][0]);
    EXPECT_DOUBLE_EQ(1.0, field(segment0)[0][1]);
}