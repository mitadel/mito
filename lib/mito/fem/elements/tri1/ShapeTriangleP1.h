// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    class ShapeTriangleP1 {

      public:
        // the reference element
        using reference_element_type = geometry::reference_triangle_t;
        // the barycentric coordinates type
        using barycentric_coordinates_type =
            typename reference_element_type::barycentric_coordinates_type;
        // the number of shape functions
        static constexpr int N = 3;

      private:
        // linear shape functions on the reference triangle in barycentric coordinates
        static constexpr auto xi_0 = reference_element_type::xi<0>;
        static constexpr auto xi_1 = reference_element_type::xi<1>;
        static constexpr auto xi_2 = reference_element_type::xi<2>;

        // linear shape functions on the triangle
        static constexpr auto phi_0 = xi_0;
        static constexpr auto phi_1 = xi_1;
        static constexpr auto phi_2 = xi_2;

        // the shape functions
        static constexpr auto phi = std::make_tuple(phi_0, phi_1, phi_2);

        // the gradients of the shape functions
        static constexpr auto dphi = std::make_tuple(
            functions::constant<barycentric_coordinates_type>(tensor::e_0<2>),
            functions::constant<barycentric_coordinates_type>(tensor::e_1<2>),
            functions::constant<barycentric_coordinates_type>(-tensor::e_0<2> - tensor::e_1<2>));

      public:
        // get the a-th shape function as a function of barycentric coordinates
        template <int a>
        requires(a >= 0 && a < N)
        constexpr auto shape() const
        {
            // return the a-th shape function
            return std::get<a>(phi);
        }

        // get the a-th shape function's derivative as a function of barycentric coordinates
        template <int a>
        requires(a >= 0 && a < N)
        constexpr auto dshape() const
        {
            // return the a-th shape function's gradient
            return std::get<a>(dphi);
        }
    };

}    // namespace mito


// end of file
