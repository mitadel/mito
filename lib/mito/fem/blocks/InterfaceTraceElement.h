// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    // the view of an {InterfaceElement} through a trace operator: local nodes below
    // {n_side_nodes} are on the left of the interface tangent, the rest on its right
    template <class baseElementT, trace_operator_t opT>
    class InterfaceTraceElement {

      public:
        // the interface element this is a view of
        using base_element_type = baseElementT;
        // the traits of this element (the same as those of the interface element)
        using traits = typename base_element_type::traits;
        // the parametric coordinates type
        using parametric_coordinates_type = typename base_element_type::parametric_coordinates_type;

        // the number of nodes on either side of the interface
        static constexpr int n_side_nodes = base_element_type::n_side_nodes;
        // the number of degrees of freedom coupled by this element
        static constexpr int n_nodes = base_element_type::n_nodes;

      public:
        // the constructor
        constexpr InterfaceTraceElement(const base_element_type & element) : _element(element) {}

      public:
        // get the degrees of freedom coupled by this element
        constexpr auto connectivity() const noexcept -> const auto &
        {
            return _element.connectivity();
        }

        // get the parametrized element
        constexpr auto element() const noexcept { return _element.element(); }

        // get the element parametrization
        constexpr auto parametrization() const noexcept { return _element.parametrization(); }

        // the shape function at local node {a}, combined according to {opT}
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto shape() const
        {
            return _side_factor<a>() * _element.template shape<a % n_side_nodes>();
        }

        // the gradient at local node {a}, combined the same way as {shape}
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto gradient() const
        {
            return _side_factor<a>() * _element.template gradient<a % n_side_nodes>();
        }

      private:
        // the factor local node {a} contributes with: +1 on the left and -1 on the right for the
        // jump, 1/2 on either side for the average
        template <int a>
        static constexpr auto _side_factor() -> tensor::scalar_t
        {
            if constexpr (opT == trace_operator_t::jump) {
                return (a < n_side_nodes) ? 1.0 : -1.0;
            } else {
                return 0.5;
            }
        }

      private:
        // the interface element this is a view of
        const base_element_type & _element;
    };

}    // namespace mito::fem::blocks


// end of file
