// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // advection matrix block factory
    template <class elementT, int doe, fields::vector_field_c velocityFieldT>
    constexpr auto advection(const velocityFieldT & velocity)
    {
        // return an advection matrix block
        return value_gradient_block<elementT, doe>(velocity);
    }

    // diffusion matrix block factory
    template <class elementT, int doe, fields::tensor_field_c diffusivityFieldT>
    constexpr auto diffusion(const diffusivityFieldT & diffusivity)
    {
        // return a diffusion matrix block
        return grad_grad_block<elementT, doe>(diffusivity);
    }

    // stiffness matrix block factory
    template <class elementT, int doe, fields::tensor_field_c elasticModulusFieldT>
    constexpr auto stiffness(const elasticModulusFieldT & elastic_modulus)
    {
        // return a stiffness matrix block
        return grad_grad_block<elementT, doe>(elastic_modulus);
    }

    // reaction matrix block factory
    template <class elementT, int doe, fields::scalar_field_c reactionRateFieldT>
    constexpr auto reaction(const reactionRateFieldT & reaction_rate)
    {
        // return a reaction matrix block
        return value_value_block<elementT, doe>(reaction_rate);
    }

    // mass matrix block factory
    template <class elementT, int doe, fields::scalar_field_c massDensityFieldT>
    constexpr auto mass(const massDensityFieldT & mass_density)
    {
        // return a mass matrix block
        return value_value_block<elementT, doe>(mass_density);
    }

    // source term vector block factory
    template <class elementT, int doe, fields::scalar_field_c sourceFieldT>
    constexpr auto source(const sourceFieldT & source)
    {
        // return a source term vector block
        return value_block<elementT, doe>(source);
    }

}


// end of file
