// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fem.h>


TEST(Fem, NodalField)
{
    // say we have 20 nodes
    int n_nodes = 20;

    // a field of mito vectors
    auto field = mito::fem::nodal_field<mito::vector_t<2>>(n_nodes, "field");

    // expect that the number of nodes  is 20
    EXPECT_TRUE(field.n_nodes() == n_nodes);

    // populate the field
    int i = 0;
    for (auto & vector : field) {
        if (i == 0) {
            vector = mito::vector_t<2>{ 0.0, 0.0 };
        } else {
            vector = field(i - 1) + mito::vector_t<2>{ 1, 1 };
        }
        ++i;
    }

    // expect that editing {vector} has edited the {field}
    EXPECT_TRUE((field(10) == mito::vector_t<2>{ 10.0, 10.0 }));
}