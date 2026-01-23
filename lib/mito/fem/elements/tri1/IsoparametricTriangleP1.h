// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangleP1} represents a second order simplex living in 2D cartesian space,
// equipped with linear shape functions defined in the parametric space.


namespace mito::fem {

    class IsoparametricTriangleP1 : public IsoparametricTriangle {

      public:
        // the degree of the finite element
        static constexpr int degree = 1;
        // the type of shape functions
        using shape_functions_type = ShapeTriangleP1;
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
        inline IsoparametricTriangleP1(
            const cell_type & geometric_simplex, const coordinate_system_type & coord_system,
            const connectivity_type & connectivity) :
            IsoparametricTriangle(geometric_simplex, coord_system),
            _connectivity(connectivity)
        {}

        // destructor
        inline ~IsoparametricTriangleP1() = default;

        // delete move constructor
        constexpr IsoparametricTriangleP1(IsoparametricTriangleP1 &&) noexcept = delete;

        // delete copy constructor
        constexpr IsoparametricTriangleP1(const IsoparametricTriangleP1 &) = delete;

        // delete assignment operator
        constexpr IsoparametricTriangleP1 & operator=(const IsoparametricTriangleP1 &) = delete;

        // delete move assignment operator
        constexpr IsoparametricTriangleP1 & operator=(IsoparametricTriangleP1 &&) noexcept = delete;

      public:
        // get the discretization nodes
        constexpr auto connectivity() const noexcept -> const connectivity_type &
        {
            return _connectivity;
        }

        // get the shape function associated with local node {a}
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto shape() const
        {
            // return the shape functions
            return shape_functions.shape<a>();
        }

        // get the jacobian of the isoparametric mapping from barycentric to actual coordinates
        constexpr auto jacobian() const
        {
            // assemble the jacobian as a function of barycentric coordinates
            auto jacobian_function = functions::function(
                [&](const parametric_coordinates_type & xi) -> tensor::matrix_t<2> {
                    // get the shape functions derivatives
                    constexpr auto dphi_0 = shape_functions.dshape<0>();
                    constexpr auto dphi_1 = shape_functions.dshape<1>();
                    constexpr auto dphi_2 = shape_functions.dshape<2>();

                    // compute the gradient of the isoparametric mapping
                    return (
                        tensor::dyadic(_x0, dphi_0(xi)) + tensor::dyadic(_x1, dphi_1(xi))
                        + tensor::dyadic(_x2, dphi_2(xi)));
                });

            // and return it
            return jacobian_function;
        }

        // get the gradient of the a-th shape function as a function of barycentric coordinates
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto gradient() const
        {
            // assemble the gradient as a function of barycentric coordinates
            auto gradient_function = functions::function(
                [&](const parametric_coordinates_type & xi) -> tensor::vector_t<2> {
                    // the jacobian of the mapping from the reference element to the physical
                    // element evaluated at {xi}
                    auto J = jacobian()(xi);
                    // the derivative of the coordinates with respect to the barycentric coordinates
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
