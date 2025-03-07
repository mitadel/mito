// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// Differential operators on Fields
namespace mito::fields {

    // the {I...}-th first partial derivative of a field
    template <int... I>
    constexpr auto derivative(const field_c auto & f)
    {
        // the {I...}-th first partial derivative
        return field(mito::functions::derivative<I...>(f.function()));
    }

    // function to compute the gradient of a scalar field
    template <scalar_field_c F>
    constexpr auto gradient(const F & field)
    {
        // the type of coordinate
        using coordinate_t = F::coordinates_type;
        // the spatial dimension of the field
        constexpr int D = coordinate_t::dim;

        // helper function to compute the gradient of a scalar
        constexpr auto _grad = []<size_t... I>(const F & field, std::index_sequence<I...>) {
            // the vector of the partial derivatives
            return ((derivative<I>(field) * uniform_field<coordinate_t>(tensor::e<I, D>)) + ...);
        };

        return _grad(field, std::make_index_sequence<D>{});
    }

    // function to compute the gradient of a vector field
    template <vector_field_c F>
    constexpr auto gradient(const F & field)
    {
        // the type of coordinate
        using coordinate_t = F::coordinates_type;
        // the spatial dimension of the field
        constexpr int D = coordinate_t::dim;

        // helper function to compute the gradient of a scalar
        constexpr auto _grad = []<size_t... J>(const F & field, std::index_sequence<J...>) {
            constexpr auto _grad_with_respect_to = []<size_t K, size_t... I>(
                                                       const F & field, std::index_sequence<I...>) {
                // the tensor of the partial derivatives
                // (\partial field_I / \partial x_K ) * e_IK
                return (
                    (derivative<K>(field * uniform_field<coordinate_t>(tensor::e<I, D>))
                     * uniform_field<coordinate_t>(tensor::unit<tensor::matrix_t<D>, I, K>))
                    + ...);
            };

            return (
                (_grad_with_respect_to.template operator()<J>(field, std::make_index_sequence<D>{}))
                + ...);
        };

        return _grad(field, std::make_index_sequence<D>{});
    }

    // function to compute the divergence of a vector field
    template <vector_field_c F>
    constexpr auto divergence(const F & field)
    {
        // the type of coordinate
        using coordinate_t = F::coordinates_type;
        // the spatial dimension of the field
        constexpr int D = coordinate_t::dim;

        // helper function to compute the divergence of a vector field
        constexpr auto _div = []<size_t... I>(const F & field, std::index_sequence<I...>) {
            // the summation of the I-th partial derivative of the I-th component
            return ((derivative<I>(field * uniform_field<coordinate_t>(tensor::e<I, D>))) + ...);
        };

        // all done
        return _div(field, std::make_index_sequence<D>{});
    }

    // function to compute the divergence of a tensor field
    template <tensor_field_c F>
    constexpr auto divergence(const F & field)
    {
        // the type of coordinate
        using coordinate_t = F::coordinates_type;
        // the spatial dimension of the field
        constexpr int D = coordinate_t::dim;

        // helper function to compute the gradient of a scalar
        constexpr auto _div = []<size_t... J>(const F & field, std::index_sequence<J...>) {
            constexpr auto _div_with_respect_to = []<size_t K, size_t... I>(
                                                      const F & field, std::index_sequence<I...>) {
                // the vector of the partial derivatives
                // (\partial field_KI / \partial x_I) * e_K
                return (
                    (derivative<I>(
                         (field * uniform_field<coordinate_t>(tensor::e<I, D>))
                         * uniform_field<coordinate_t>(tensor::e<K, D>))
                     * uniform_field<coordinate_t>(tensor::e<K, D>))
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
