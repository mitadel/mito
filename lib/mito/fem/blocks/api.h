// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // advection matrix block factory
    template <
        class elementT, int doe = 2 * elementT::degree - 1, fields::vector_field_c velocityFieldT>
    constexpr auto advection(const velocityFieldT & velocity);

    // diffusion matrix block factory
    template <
        class elementT, int doe = 2 * elementT::degree - 1,
        fields::tensor_field_c diffusivityFieldT>
    constexpr auto diffusion(const diffusivityFieldT & diffusivity);

    // stiffness matrix block factory
    template <
        class elementT, int doe = 2 * elementT::degree - 1,
        fields ::tensor_field_c elasticModulusFieldT>
    constexpr auto stiffness(const elasticModulusFieldT & elastic_modulus);

    // reaction matrix block factory
    template <
        class elementT, int doe = 2 * elementT::degree, fields::scalar_field_c reactionRateFieldT>
    constexpr auto reaction(const reactionRateFieldT & reaction_rate);

    // mass matrix block factory
    template <
        class elementT, int doe = 2 * elementT::degree, fields::scalar_field_c massDensityFieldT>
    constexpr auto mass(const massDensityFieldT & mass_density);

    // source term vector block factory
    template <class elementT, int doe = 2 * elementT::degree, fields::scalar_field_c sourceFieldT>
    constexpr auto source(const sourceFieldT & source);

    // jump-jump matrix block factory (the transverse conductance of a collapsed layer)
    template <
        class elementT, int doe = 2 * elementT::degree, fields::scalar_field_c conductanceFieldT>
    constexpr auto jump_jump(const conductanceFieldT & conductance);

    // average-average gradient matrix block factory (the tangential conductance of a collapsed
    // layer)
    template <
        class elementT, int doe = 2 * elementT::degree - 1,
        fields::tensor_field_c conductivityFieldT>
    constexpr auto average_average_gradient(const conductivityFieldT & conductivity);

}


// end of file
