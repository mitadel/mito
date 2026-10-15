// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;
// the material type
using material_t = mito::materials::thermal::ConstantIsotropic<2>;
// the model type
using model_t = mito::models::HeatDiffusion<material_t>;

TEST(Models, SEMI_DISCRETE_WEAKFORM_FACTORY)
{
    // constant isotropic material with density = 1.25, conductivity = 0.5, specific heat = 2,
    constexpr auto conductivity = mito::real(0.5);
    constexpr auto density = mito::real(1.25);
    constexpr auto specific_heat = mito::real(2.0);
    constexpr auto source_term = mito::real(1.0);

    // the material type
    auto material = material_t(conductivity, density, specific_heat, source_term);

    // the model
    auto model = model_t(material);

    // the semi-discrete weakform
    auto weakform = mito::fem::semi_discrete_weakform(model);

    // all done
    return;
}