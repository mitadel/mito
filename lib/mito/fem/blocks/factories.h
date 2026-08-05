// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // advection matrix block factory
    template <class elementT, fields::vector_field_c coefficientFieldT>
    constexpr auto advection_block(const coefficientFieldT & coefficient)
    {
        // all done
        return value_gradient_block<elementT, coefficientFieldT>(coefficient);
    }

    // diffusion matrix block factory
    template <class elementT, fields::tensor_field_c coefficientFieldT>
    constexpr auto diffusion_block(const coefficientFieldT & coefficient)
    {
        // all done
        return grad_grad_block<elementT, coefficientFieldT>(coefficient);
    }

    // stiffness matrix block factory
    template <class elementT, fields::tensor_field_c coefficientFieldT>
    constexpr auto stiffness_block(const coefficientFieldT & coefficient)
    {
        // all done
        return grad_grad_block<elementT, coefficientFieldT>(coefficient);
    }

    // reaction matrix block factory
    template <class elementT>
    constexpr auto reaction_block()
    {
        // all done
        return value_value_block<elementT>();
    }

    // mass matrix block factory
    template <class elementT>
    constexpr auto mass_block()
    {
        // all done
        return value_value_block<elementT>();
    }

    // source term vector block factory
    template <class elementT, int doe, fields::scalar_field_c coefficientFieldT>
    constexpr auto source_term_block(const coefficientFieldT & coefficient)
    {
        // all done
        return value_block<elementT, coefficientFieldT>(coefficient);
    }

    // L2 norm block factory
    template <class elementT, int doe, functions::function_c functionT>
    constexpr auto l2_norm_block(const functionT & f)
    {
        // select an appropriate quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return l2_norm_block_t<elementT, quadrature_rule_type, functionT>(f);
    }

}


// end of file
