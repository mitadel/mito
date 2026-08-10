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

    // L2 norm block factory
    template <class elementT, int doe = 2 * elementT::degree, functions::function_c functionT>
    constexpr auto l2_norm(const functionT & f)
    {
        // select an appropriate quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return l2_norm_block_t<elementT, quadrature_rule_type, functionT>(f);
    }

}


// end of file
