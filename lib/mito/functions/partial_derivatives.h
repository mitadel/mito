// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// Partial derivatives


namespace mito::functions {

    template <scalar_function_c F>
    constexpr auto derivative(const F & f)
    {
        return derivative<0>(f);
    }

    namespace {
        // implementation of the assembly of the matrix of partial derivatives
        // (row: output index; column: input index)
        template <function_c F, int D, int N>
        constexpr auto derivative_impl(const F & f)
        {
            // the output matrix type
            using output_matrix = tensor::matrix_t<N, D>;

            // helper function to compute the derivative
            constexpr auto _derivative = []<size_t... J>(const F & f, std::index_sequence<J...>) {
                constexpr auto _derivative_with_respect_to =
                    []<size_t K, size_t... I>(const F & f, std::index_sequence<I...>) {
                        // the matrix of the partial derivatives
                        // (\partial field_I / \partial x_K ) * e_IK, I = 0, ..., N-1, K = 0, ...,
                        // D-1
                        if constexpr (N == 1) {
                            return ((derivative<K>(f) * tensor::unit<output_matrix, I, K>) +...);
                        } else {
                            return ((derivative<K>(f[I]) * tensor::unit<output_matrix, I, K>) +...);
                        }
                    };

                return (
                    (_derivative_with_respect_to.template operator()<J>(
                        f, std::make_index_sequence<N>{}))
                    + ...);
            };

            return _derivative(f, std::make_index_sequence<D>{});
        }
    }

    // the derivative of a tensor-valued function of tensors
    template <tensor_function_c F>
    constexpr auto derivative(const F & f)
    {
        // the number of components of the input vector of F
        constexpr int D = F::input_type::size;
        // the number of components of the output vector of F
        constexpr int N = F::output_type::size;
        // call the implementation
        return derivative_impl<F, D, N>(f);
    }

    // the derivative of a scalar-valued function of tensors
    template <function_c F>
    requires(tensor_domain_function_c<F> and scalar_valued_function_c<F>)
    constexpr auto derivative(const F & f)
    {
        // the number of components of the input vector of F
        constexpr int D = F::input_type::size;
        // the number of components of the output vector of F
        constexpr int N = 1;
        // call the implementation
        return derivative_impl<F, D, N>(f);
    }

    // the derivative of a tensor-valued function of scalars
    template <function_c F>
    requires(scalar_domain_function_c<F> and tensor_valued_function_c<F>)
    constexpr auto derivative(const F & f)
    {
        // the number of components of the input vector of F
        constexpr int D = 1;
        // the number of components of the output vector of F
        constexpr int N = F::output_type::size;
        // call the implementation
        return derivative_impl<F, D, N>(f);
    }
}


// end of file
