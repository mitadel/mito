// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/materials.h>


TEST(Gent, TestGent)
{
    // make a channel
    journal::info_t channel("tests.gent");

    // can either use this syntax...
    // mito::materials::gent_t material(1.0 /*rho*/, 1.0 /*kappa*/, 1.0 /*mu*/, 1.0 /*Jm*/);
    // ... or this syntax
    auto material = mito::materials::gent(1.0 /*rho*/, 1.0 /*kappa*/, 1.0 /*mu*/, 1.0 /*Jm*/);

    mito::vector_t<3> u = { 0.0, 0.0, 0.0 };
    mito::matrix_t<3> Du = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
    mito::matrix_t<3> P = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    material.Constitutive(u, Du, P);

    mito::vector_t<2> u2 = { 0.0, 0.0 };
    mito::matrix_t<2> Du2 = { 1.0, 0.0, 0.0, 1.0 };
    mito::matrix_t<2> P2 = { 0.0, 0.0, 0.0, 0.0 };
    material.Constitutive(u2, Du2, P2);

    channel << "P = " << P << journal::endl;
    channel << "P2 = " << P2 << journal::endl;
}
