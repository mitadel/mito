// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // compute L2 norm on a given function space of the difference between two localizable fields
    template <class quadratureRuleT, class functionSpaceT, class F1, class F2>
    requires(
        localizable_field_c<F1, typename functionSpaceT::element_type>
        && localizable_field_c<F2, typename functionSpaceT::element_type>)
    constexpr auto compute_l2_norm(
        const functionSpaceT & function_space, const F1 & u1, const F2 & u2) -> tensor::scalar_t
    {
        // get the element type
        using element_type = typename functionSpaceT::element_type;

        // initialize the norm
        auto norm = tensor::scalar_t{ 0.0 };

        // loop on all the elements of the function space
        for (const auto & element : function_space.elements()) {
            // localize {u1} on this element
            auto u1_local = localize(u1, element);
            // localize {u2} on this element
            auto u2_local = localize(u2, element);
            // compute the elementary contribution to the norm
            norm += blocks::l2_norm_block<element_type, quadratureRuleT>(u1_local - u2_local)
                        .compute(element);
        }

        // take the square root of the accumulated norm
        return std::sqrt(norm);
    }

    // compute H1 norm on a given function space of the difference between two localizable fields
    template <class quadratureRuleT, class functionSpaceT, class F1, class F2>
    requires(
        localizable_field_c<F1, typename functionSpaceT::element_type>
        && localizable_field_c<F2, typename functionSpaceT::element_type>)
    constexpr auto compute_h1_norm(
        const functionSpaceT & function_space, const F1 & u1, const F2 & u2) -> tensor::scalar_t
    {
        // get the element type
        using element_type = typename functionSpaceT::element_type;

        // initialize the norm
        auto norm = tensor::scalar_t{ 0.0 };

        // loop on all the elements of the function space
        for (const auto & element : function_space.elements()) {
            // localize {u1} on this element
            auto u1_local = localize(u1, element);
            // localize {u2} on this element
            auto u2_local = localize(u2, element);
            // assemble the gradient of the solution field on this element
            auto u1_local_gradient = fields::gradient(u1_local);
            // localize the gradient of the exact solution on this element
            auto u2_local_gradient = fields::gradient(u2_local);
            // compute the elementary contributions to the H1 norm
            norm += blocks::l2_norm_block<element_type, quadratureRuleT>(u1_local - u2_local)
                        .compute(element)
                  + blocks::l2_norm_block<element_type, quadratureRuleT>(
                        u1_local_gradient - u2_local_gradient)
                        .compute(element);
        }

        // take the square root of the accumulated norm
        return std::sqrt(norm);
    }

}


// end of file
