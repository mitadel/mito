// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricSegmentP1} represents a first order simplex (segment) living in 1D cartesian space,
// equipped with linear shape functions defined in the parametric space.


namespace mito::fem {

    class IsoparametricSegmentP1 : public IsoparametricSegment {

      public:
        // the degree of the finite element
        static constexpr int degree = 1;
        // the type of shape functions
        using shape_functions_type = ShapeSegmentP1;
        // the canonical element type
        using canonical_element_type = typename shape_functions_type::reference_element_type;
        // the parametric coordinates type
        using parametric_coordinates_type =
            typename canonical_element_type::parametric_coordinates_type;
        // the linear shape functions
        static constexpr auto shape_functions = shape_functions_type();
        // the number of discretization discretization nodes
        static constexpr int n_nodes = shape_functions_type::N;
        // a collection of discretization discretization nodes
        using connectivity_type = std::array<discretization_node_type, n_nodes>;

      public:
        // the default constructor
        inline IsoparametricSegmentP1(
            const cell_type & geometric_simplex, const coordinate_system_type & coord_system,
            const connectivity_type & connectivity) :
            IsoparametricSegment(geometric_simplex, coord_system),
            _connectivity(connectivity)
        {}

        // destructor
        inline ~IsoparametricSegmentP1() = default;

        // delete move constructor
        constexpr IsoparametricSegmentP1(IsoparametricSegmentP1 &&) noexcept = delete;

        // delete copy constructor
        constexpr IsoparametricSegmentP1(const IsoparametricSegmentP1 &) = delete;

        // delete assignment operator
        constexpr IsoparametricSegmentP1 & operator=(const IsoparametricSegmentP1 &) = delete;

        // delete move assignment operator
        constexpr IsoparametricSegmentP1 & operator=(IsoparametricSegmentP1 &&) noexcept = delete;

      public:
        // get the discretization nodes
        constexpr auto connectivity() const noexcept -> const connectivity_type &
        {
            return _connectivity;
        }

        // get the isoparametric mapping from parametric coordinates to physical coordinates
        constexpr auto parametrization() const
        {
            // get the shape functions
            constexpr auto phi_0 = shape_functions.shape<0>();
            constexpr auto phi_1 = shape_functions.shape<1>();

            // return the isoparametric mapping from parametric to physical coordinates
            return mito::fields::linear_combination(
                std::array{ _x0, _x1 }, phi_0, phi_1);
        }

        // get the shape function associated with local node {a}
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto shape() const
        {
            // return the shape functions
            return shape_functions.shape<a>();
        }

        // get the jacobian of the isoparametric mapping from parametric to actual coordinates
        constexpr auto jacobian() const
        {
            // assemble the jacobian as a function of parametric coordinates
            auto jacobian_function = functions::function(
                [&](const parametric_coordinates_type & xi) -> tensor::matrix_t<1> {
                    // get the shape functions derivatives
                    constexpr auto dphi_0 = shape_functions.dshape<0>();
                    constexpr auto dphi_1 = shape_functions.dshape<1>();

                    // compute the jacobian of the isoparametric mapping: dx/dxi
                    auto dx_dxi = _x0 * dphi_0(xi) + _x1 * dphi_1(xi);
                    // wrap the result in a 1x1 matrix
                    return tensor::matrix_t<1>{ dx_dxi };
                });

            // and return it
            return jacobian_function;
        }

        // get the gradient of the a-th shape function as a function of parametric coordinates
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto gradient() const
        {
            // assemble the gradient as a function of parametric coordinates
            auto gradient_function =
                fields::field([&](const parametric_coordinates_type & xi) -> tensor::vector_t<1> {
                    // the jacobian of the mapping from the reference element to the physical
                    // element evaluated at {xi}
                    auto J = jacobian()(xi);
                    // the derivative of the coordinates with respect to the parametric coordinates
                    auto J_inv = tensor::inverse(J);
                    // return the spatial gradients of the shape functions evaluated at {xi}
                    return shape_functions.dshape<a>()(xi) * J_inv;
                });
            // and return it
            return gradient_function;
        }

      private:
        // the discretization nodes of the simplex
        const connectivity_type _connectivity;
    };

}    // namespace mito


// end of file
