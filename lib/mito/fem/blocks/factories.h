// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // advection matrix block factory
    template <class elementT, fields::vector_field_c velocityFieldT>
    constexpr auto advection_block(const velocityFieldT & velocity)
    {
        // degree of exactness for the quadrature rule
        // (required to integrate exactly advection product of element shape functions)
        constexpr int doe = 2 * elementT::degree - 1;

        // assemble the GAUSS quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return advection_block_t<elementT, quadrature_rule_type, velocityFieldT>(velocity);
    }

    // grad-grad matrix block factory
    template <class elementT>
    constexpr auto grad_grad_block()
    {
        // degree of exactness for the quadrature rule
        // (required to integrate exactly grad-grad product of element shape functions)
        constexpr int doe = 2 * (elementT::degree - 1);

        // assemble the GAUSS quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return grad_grad_block_t<elementT, quadrature_rule_type>();
    }

    // mass matrix block factory
    template <class elementT>
    constexpr auto mass_block()
    {
        // degree of exactness for the quadrature rule
        // (required to integrate exactly product of element shape functions)
        constexpr int doe = 2 * elementT::degree;

        // select an appropriate quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return mass_block_t<elementT, quadrature_rule_type>();
    }

    // source term block factory
    template <class elementT, int doe, fields::scalar_field_c sourceFieldT>
    constexpr auto source_term_block(const sourceFieldT & f)
    {
        // select an appropriate quadrature rule for the block
        using quadrature_rule_type = quadrature::quadrature_rule_t<
            quadrature::GAUSS, typename elementT::mesh_cell_type::reference_simplex_type, doe>;

        // all done
        return source_term_block_t<elementT, quadrature_rule_type, sourceFieldT>(f);
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
