// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// Differential operators on Fields
namespace mito::fields {

    // function to compute the gradient of a scalar field
    template <scalar_field_c F>
    constexpr auto gradient(const F & field)
    {
        // the type of coordinate
        using coordinate_t = F::input_type;
        // the spatial dimension of the field
        constexpr int D = coordinate_t::dim;

        // helper function to compute the gradient of a scalar
        constexpr auto _grad = []<size_t... I>(const F & field, std::index_sequence<I...>) {
            // the vector of the partial derivatives
            return (
                (functions::derivative<I>(field)
                 * functions::constant<coordinate_t>(tensor::e<I, D>))
                + ...);
        };

        return _grad(field, std::make_index_sequence<D>{});
    }

    // function to compute the gradient of a vector field
    template <vector_field_c F>
    constexpr auto gradient(const F & field)
    {
        // the type of coordinate
        using coordinate_t = F::input_type;
        // the spatial dimension of the field
        constexpr int D = coordinate_t::dim;
        // the number of components of the vector field
        constexpr int N = F::output_type::size;

        // helper function to compute the gradient of a scalar
        constexpr auto _grad = []<size_t... J>(const F & field, std::index_sequence<J...>) {
            constexpr auto _grad_with_respect_to = []<size_t K, size_t... I>(
                                                       const F & field, std::index_sequence<I...>) {
                // the tensor of the partial derivatives
                // (\partial field_I / \partial x_K ) * e_IK, I = 0, ..., N-1, K = 0, ..., D-1
                return (
                    (functions::derivative<K>(
                         field * functions::constant<coordinate_t>(tensor::e<I, N>))
                     * functions::constant<coordinate_t>(
                         tensor::unit<tensor::matrix_t<N, D>, I, K>))
                    + ...);
            };

            return (
                (_grad_with_respect_to.template operator()<J>(field, std::make_index_sequence<N>{}))
                + ...);
        };

        return _grad(field, std::make_index_sequence<D>{});
    }

    // function to compute the divergence of a vector field
    template <vector_field_c F>
    constexpr auto divergence(const F & field)
    {
        // the type of coordinate
        using coordinate_t = F::input_type;
        // the spatial dimension of the field
        constexpr int D = coordinate_t::dim;

        // helper function to compute the divergence of a vector field
        constexpr auto _div = []<size_t... I>(const F & field, std::index_sequence<I...>) {
            // the summation of the I-th partial derivative of the I-th component
            return (
                (functions::derivative<I>(
                    field * functions::constant<coordinate_t>(tensor::e<I, D>)))
                + ...);
        };

        // all done
        return _div(field, std::make_index_sequence<D>{});
    }

    // function to compute the divergence of a tensor field
    template <tensor_field_c F>
    constexpr auto divergence(const F & field)
    {
        // the type of coordinate
        using coordinate_t = F::input_type;
        // the spatial dimension of the field
        constexpr int D = coordinate_t::dim;

        // helper function to compute the gradient of a scalar
        constexpr auto _div = []<size_t... J>(const F & field, std::index_sequence<J...>) {
            constexpr auto _div_with_respect_to = []<size_t K, size_t... I>(
                                                      const F & field, std::index_sequence<I...>) {
                // the vector of the partial derivatives
                // (\partial field_KI / \partial x_I) * e_K
                return (
                    (functions::derivative<I>(
                         (field * functions::constant<coordinate_t>(tensor::e<I, D>))
                         * functions::constant<coordinate_t>(tensor::e<K, D>))
                     * functions::constant<coordinate_t>(tensor::e<K, D>))
                    + ...);
            };

            return (
                (_div_with_respect_to.template operator()<J>(field, std::make_index_sequence<D>{}))
                + ...);
        };

        return _div(field, std::make_index_sequence<D>{});
    }
}


// end of file
