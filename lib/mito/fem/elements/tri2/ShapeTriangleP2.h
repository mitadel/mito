// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    class ShapeTriangleP2 {

      public:
        // the reference element
        using reference_element_type = geometry::reference_triangle_t;
        // the number of shape functions
        static constexpr int N = 6;

      private:
        // get the parametric coordinates from the reference element
        static constexpr auto xi_0 = reference_element_type::xi<0>;
        static constexpr auto xi_1 = reference_element_type::xi<1>;
        static constexpr auto xi_2 = 1.0 - xi_0 - xi_1;

        // quadratic shape functions on the triangle
        static constexpr auto phi_3 = 4.0 * xi_0 * xi_1;
        static constexpr auto phi_4 = 4.0 * xi_1 * xi_2;
        static constexpr auto phi_5 = 4.0 * xi_0 * xi_2;
        static constexpr auto phi_0 = xi_0 - 0.5 * phi_5 - 0.5 * phi_3;
        static constexpr auto phi_1 = xi_1 - 0.5 * phi_3 - 0.5 * phi_4;
        static constexpr auto phi_2 = xi_2 - 0.5 * phi_5 - 0.5 * phi_4;

        // the shape functions
        static constexpr auto phi = std::make_tuple(phi_0, phi_1, phi_2, phi_3, phi_4, phi_5);

        // the gradients of the shape functions
        static constexpr auto dphi = std::make_tuple(
            tensor::e_0<2> * (1.0 - 2.0 * xi_1 - 2.0 * xi_2 + 2.0 * xi_0),
            tensor::e_1<2> * (1.0 - 2.0 * xi_0 - 2.0 * xi_2 + 2.0 * xi_1),
            (tensor::e_0<2> + tensor::e_1<2>) *(-1.0 - 2.0 * xi_2 + 2.0 * xi_0 + 2.0 * xi_1),
            tensor::e_0<2> * (4.0 * xi_1) + tensor::e_1<2> * (4.0 * xi_0),
            tensor::e_0<2> * (-4.0 * xi_1) + tensor::e_1<2> * (4.0 * xi_2 - 4.0 * xi_1),
            tensor::e_0<2> * (4.0 * xi_2 - 4.0 * xi_0) + tensor::e_1<2> * (-4.0 * xi_0));

      public:
        // get the a-th shape function as a function of barycentric coordinates
        template <int a>
        requires(a >= 0 && a < N)
        constexpr auto shape() const
        {
            // return the a-th shape function
            return std::get<a>(phi);
        }

        // get the a-th shape function's gradient as a function of barycentric coordinates
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
