// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    template <
        int N, coordinates_c coordsT, coordinates_c parametric_coordsT, class JacobianFieldT,
        class ParametrizationT>
    class InducedMetric {

      public:
        // ambient dimension
        static constexpr int D = coordsT::dim;

        // verify parametric coordinates have correct dimension
        static_assert(
            parametric_coordsT::dim == N,
            "Parametric coordinates dimension must match submanifold dimension N");

        // the metric space for the ambient coordinates
        using ambient_metric_space_type = metric_space<coordsT>;
        // the metric space for the parametric coordinates
        using parametric_metric_space_type = metric_space<parametric_coordsT>;
        // parametric coordinates type
        using parametric_coordinates_type = parametric_coordsT;
        // physical (ambient) coordinates
        using coordinates_type = coordsT;
        // induced metric tensor type (N x N)
        using induced_metric_tensor_type = tensor::matrix_t<N, N>;
        // gradient vector type in ambient space
        using gradient_vector_type = tensor::vector_t<D>;

      private:
        JacobianFieldT _jacobian;
        ParametrizationT _parametrization;

      public:
        constexpr InducedMetric(JacobianFieldT jacobian, ParametrizationT parametrization) :
            _jacobian(std::move(jacobian)),
            _parametrization(std::move(parametrization))
        {}

      public:
        // Induced metric tensor at parametric point: g_induced = J^T * g * J
        constexpr auto g(const parametric_coordinates_type & xi) const -> induced_metric_tensor_type
        {
            auto x = _parametrization(xi);
            auto J = _jacobian(xi);
            auto g_ambient = ambient_metric_space_type::g(x);
            return tensor::transpose(J) * g_ambient * J;
        }

        // Inverse induced metric at parametric point
        constexpr auto g_inv(const parametric_coordinates_type & xi) const
            -> induced_metric_tensor_type
        {
            return tensor::inverse(g(xi));
        }

        // Volume element: sqrt(det(g_induced))
        constexpr auto volume_element(const parametric_coordinates_type & xi) const
            -> tensor::scalar_t
        {
            return std::sqrt(tensor::determinant(g(xi)));
        }

        // Contravariant gradient in ambient space: J * g_induced^{-1} * dphi/dxi
        // For N=1, dphi_dxi is a scalar; for N>1, dphi_dxi is vector_t<N>
        constexpr auto gradient(
            const tensor::scalar_t & dphi_dxi,
            const parametric_coordinates_type & xi) const -> gradient_vector_type
        requires(N == 1)
        {
            auto J = _jacobian(xi);
            auto g_inv_induced = g_inv(xi);
            auto dphi_vec = tensor::vector_t<1>{ dphi_dxi };
            return J * (g_inv_induced * dphi_vec);
        }

        constexpr auto gradient(
            const tensor::vector_t<N> & dphi_dxi,
            const parametric_coordinates_type & xi) const -> gradient_vector_type
        requires(N > 1)
        {
            auto J = _jacobian(xi);
            auto g_inv_induced = g_inv(xi);
            return J * (g_inv_induced * dphi_dxi);
        }

      public:
        // Basis elements for the parametric space (static, from parametric metric space)
        template <int I>
        requires(I >= 0 && I < N)
        static constexpr auto e = parametric_metric_space_type::template e<I>;

      public:
        // Basis one-forms in parametric space (standard Euclidean dual basis)
        template <int I>
        requires(I >= 0 && I < N)
        static constexpr auto dx = parametric_metric_space_type::template dx<I>;

      private:
        // Helper to wedge all N basis one-forms
        template <int... J>
        static constexpr auto _wedge(tensor::integer_sequence<J...>)
        requires(sizeof...(J) == N)
        {
            // Wedge all dx^J one-forms
            return fields::wedge(dx<J>...);
        }

      public:
        // The induced metric volume form (N-form on parametric domain)
        // w = sqrt(det(g_induced)) * (dx^0 ∧ dx^1 ∧ ... ∧ dx^(N-1))
        constexpr auto w() const
        {
            // Create volume element field
            auto volume_element_field =
                functions::function([this](const parametric_coordinates_type & xi) {
                    return this->volume_element(xi);
                });

            // Wedge the basis one-forms
            auto wedge_forms = _wedge(tensor::make_integer_sequence<N>{});

            // Return scaled wedge product
            return volume_element_field * wedge_forms;
        }
    };

}    // namespace mito::geometry


// end of file
