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

    // L2 norm block
    template <class elementT, class quadratureRuleT, functions::function_c functionT>
    using l2_norm_block_t = L2NormBlock<elementT, quadratureRuleT, functionT>;

    // L2 norm block factory
    template <class elementT, int doe, functions::function_c functionT>
    constexpr auto l2_norm(const functionT & f);
}


// end of file
