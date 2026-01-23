// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    class ShapeSegmentP1 {

      public:
        // the reference element
        using reference_element_type = geometry::reference_segment_t;
        // the number of shape functions
        static constexpr int N = 2;

      private:
        // linear shape functions on the reference segment in parametric coordinates
        static constexpr auto xi_0 = reference_element_type::xi<0>;

        // linear shape functions on the segment
        static constexpr auto phi_0 = 1.0 - xi_0;
        static constexpr auto phi_1 = xi_0;

        // the shape functions
        static constexpr auto phi = std::make_tuple(phi_0, phi_1);

        // the gradients of the shape functions
        static constexpr auto dphi = std::make_tuple(
            fields::gradient(phi_0), fields::gradient(phi_1));

      public:
        // get the a-th shape function as a function of parametric coordinates
        template <int a>
        requires(a >= 0 && a < N)
        constexpr auto shape() const
        {
            // return the a-th shape function
            return std::get<a>(phi);
        }

        // get the a-th shape function's derivative as a function of parametric coordinates
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
