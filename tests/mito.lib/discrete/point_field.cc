// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/discrete.h>


// pick a dimension in space
constexpr auto D = 3;
// a type for the field
using vector_t = mito::tensor::vector_t<2>;
// assemble the quadrature field
using point_field_t = mito::discrete::point_field_t<D, vector_t>;

// fetch the point cloud
auto & cloud = mito::geometry::point_cloud<D>();
// build a point
auto point = cloud.point();


void
constFunction(const point_field_t & field)
{
    // get a (deep) copy of the vector field at {point}
    auto vector = field(point);

    // edit the copy
    vector[1] = 10;

    // all done
    return;
}

TEST(Discretization, PointFields)
{
    // a point field
    auto field = point_field_t("field");

    // insert one entry using the default value
    field.insert(point);

    // get a reference the value of the vector field at {point}
    auto & vector = field(point);

    // expect that the vector size is 2
    EXPECT_TRUE(vector.size == 2);

    // modify the content of {vector}
    vector[0] = 0.0;
    vector[1] = 1.0;

    // expect that editing {vector} has edited the {field}
    EXPECT_DOUBLE_EQ(0.0, field(point)[0]);
    EXPECT_DOUBLE_EQ(1.0, field(point)[1]);

    // call a function that modifies a (deep) copy of the field
    constFunction(field);

    // expect that the {field} is unchanged (deep copy vs. shallow copy)
    EXPECT_DOUBLE_EQ(0.0, field(point)[0]);
    EXPECT_DOUBLE_EQ(1.0, field(point)[1]);
}