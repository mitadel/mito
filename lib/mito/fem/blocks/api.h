// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // advection matrix block factory
    template <class elementT, int doe, fields::vector_field_c coefficientFieldT>
    constexpr auto advection(const coefficientFieldT & coefficient);

    // diffusion matrix block factory
    template <class elementT, int doe, fields::tensor_field_c coefficientFieldT>
    constexpr auto diffusion(const coefficientFieldT & coefficient);

    // stiffness matrix block factory
    template <class elementT, int doe, fields::tensor_field_c coefficientFieldT>
    constexpr auto stiffness(const coefficientFieldT & coefficient);

    // reaction matrix block factory
    template <class elementT, int doe, fields::scalar_field_c coefficientFieldT>
    constexpr auto reaction(const coefficientFieldT & coefficient);

    // mass matrix block factory
    template <class elementT, int doe, fields::scalar_field_c coefficientFieldT>
    constexpr auto mass(const coefficientFieldT & coefficient);

    // source term vector block factory
    template <class elementT, int doe, fields::scalar_field_c coefficientFieldT>
    constexpr auto source(const coefficientFieldT & coefficient);

}


// end of file
