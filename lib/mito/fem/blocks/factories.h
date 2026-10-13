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

    // jump-jump matrix block factory (the transverse conductance of a collapsed layer)
    template <class elementT, int doe, fields::scalar_field_c conductanceFieldT>
    constexpr auto jump_jump(const conductanceFieldT & conductance)
    {
        // a value value block, computed on the jump of the element's shape functions
        auto block = value_value_block<elementT, doe>(conductance);
        return InterfaceBlock<decltype(block), trace_operator_t::jump>(block);
    }

    // average-average gradient matrix block factory (the tangential conductance of a collapsed
    // layer)
    template <class elementT, int doe, fields::tensor_field_c conductivityFieldT>
    constexpr auto average_average_gradient(const conductivityFieldT & conductivity)
    {
        // a grad grad block, computed on the average of the element's shape functions
        auto block = grad_grad_block<elementT, doe>(conductivity);
        return InterfaceBlock<decltype(block), trace_operator_t::average>(block);
    }

}


// end of file
