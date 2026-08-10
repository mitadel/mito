// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // advection matrix block factory
    template <
        class elementT, int doe = 2 * elementT::degree - 1,
        fields::vector_field_c coefficientFieldT>
    constexpr auto advection(const coefficientFieldT & coefficient)
    {
        // all done
        return value_gradient_block<elementT, doe>(coefficient);
    }

    // diffusion matrix block factory
    template <
        class elementT, int doe = 2 * (elementT::degree - 1),
        fields::tensor_field_c coefficientFieldT>
    constexpr auto diffusion(const coefficientFieldT & coefficient)
    {
        // all done
        return grad_grad_block<elementT, doe>(coefficient);
    }

    // stiffness matrix block factory
    template <
        class elementT, int doe = 2 * (elementT::degree - 1),
        fields::tensor_field_c coefficientFieldT>
    constexpr auto stiffness(const coefficientFieldT & coefficient)
    {
        // all done
        return grad_grad_block<elementT, doe>(coefficient);
    }

    // reaction matrix block factory
    template <
        class elementT, int doe = 2 * elementT::degree, fields::scalar_field_c coefficientFieldT>
    constexpr auto reaction(const coefficientFieldT & coefficient)
    {
        // all done
        return value_value_block<elementT, doe>(coefficient);
    }

    // mass matrix block factory
    template <
        class elementT, int doe = 2 * elementT::degree, fields::scalar_field_c coefficientFieldT>
    constexpr auto mass(const coefficientFieldT & coefficient)
    {
        // all done
        return value_value_block<elementT, doe>(coefficient);
    }

    // source term vector block factory
    template <class elementT, int doe = elementT::degree, fields::scalar_field_c coefficientFieldT>
    constexpr auto source(const coefficientFieldT & coefficient)
    {
        // all done
        return value_block<elementT, doe>(coefficient);
    }

}


// end of file
