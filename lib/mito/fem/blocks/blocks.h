// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // value grad matrix block
    template <class elementT, class quadratureRuleT, fields::vector_field_c coefficientFieldT>
    using value_gradient_block_t = ValueGradientBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // value gradient matrix block factory
    template <class elementT, fields::vector_field_c coefficientFieldT>
    constexpr auto value_gradient_block(const coefficientFieldT & coefficient)
    {
        // degree of exactness for the quadrature rule
        // (required to integrate exactly the integrand with a constant coefficient)
        constexpr int doe = 2 * elementT::degree - 1;

        // assemble the GAUSS quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return value_gradient_block_t<elementT, quadrature_rule_type, coefficientFieldT>(
            coefficient);
    }

    // grad grad matrix block
    template <class elementT, class quadratureRuleT, fields::tensor_field_c coefficientFieldT>
    using grad_grad_block_t = GradientGradientBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // grad grad matrix block factory
    template <class elementT, fields::tensor_field_c coefficientFieldT>
    constexpr auto grad_grad_block(const coefficientFieldT & coefficient)
    {
        // degree of exactness for the quadrature rule
        // (required to integrate exactly the integrand with a constant coefficient)
        constexpr int doe = 2 * (elementT::degree - 1);

        // assemble the GAUSS quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return grad_grad_block_t<elementT, quadrature_rule_type, coefficientFieldT>(coefficient);
    }

    // value value matrix block
    template <class elementT, class quadratureRuleT>
    using value_value_block_t = ValueValueBlock<elementT, quadratureRuleT>;

    // value value matrix block factory
    template <class elementT>
    constexpr auto value_value_block()
    {
        // degree of exactness for the quadrature rule
        // (required to integrate exactly product of element shape functions)
        constexpr int doe = 2 * elementT::degree;

        // select an appropriate quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return value_value_block_t<elementT, quadrature_rule_type>();
    }

    // value vector block
    template <class elementT, class quadratureRuleT, fields::scalar_field_c coefficientFieldT>
    using value_block_t = ValueBlock<elementT, quadratureRuleT, coefficientFieldT>;

    // value vector block factory
    template <class elementT, int doe, fields::scalar_field_c coefficientFieldT>
    constexpr auto value_block(const coefficientFieldT & coefficient)
    {
        // select an appropriate quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return value_block_t<elementT, quadrature_rule_type, coefficientFieldT>(coefficient);
    }

}


// end of file
