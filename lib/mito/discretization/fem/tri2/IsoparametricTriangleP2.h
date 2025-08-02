// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangleP2} represents a second order simplex living in 2D cartesian space
// equipped with quadratic shape functions defined in the parametric space.


namespace mito::discretization {

    class IsoparametricTriangleP2 : public IsoparametricTriangle {

      public:
        // the degree of the finite element
        static constexpr int degree = 2;
        // the type of shape functions
        using shape_functions_type = ShapeTriangleP2;
        // the linear shape functions
        static constexpr auto shape_functions = shape_functions_type();
        // the number of discretization nodes
        static constexpr int n_nodes = shape_functions_type::N;
        // a collection of discretization nodes
        using connectivity_type = std::array<discretization_node_type, n_nodes>;

      public:
        // the default constructor
        inline IsoparametricTriangleP2(
            const geometric_simplex_type & geometric_simplex,
            const coordinate_system_type & coord_system, const connectivity_type & connectivity) :
            IsoparametricTriangle(geometric_simplex, coord_system),
            _connectivity(connectivity)
        {}

        // destructor
        ~IsoparametricTriangleP2() = default;

        // delete move constructor
        constexpr IsoparametricTriangleP2(IsoparametricTriangleP2 &&) noexcept = delete;

        // delete copy constructor
        constexpr IsoparametricTriangleP2(const IsoparametricTriangleP2 &) = delete;

        // delete assignment operator
        constexpr IsoparametricTriangleP2 & operator=(const IsoparametricTriangleP2 &) = delete;

        // delete move assignment operator
        constexpr IsoparametricTriangleP2 & operator=(IsoparametricTriangleP2 &&) noexcept = delete;

      private:
        // the isoparametric mapping from barycentric coordinates to physical coordinates
        constexpr auto _x_cell() const
        {
            auto x3 = 0.5 * (_x0 + _x1);
            auto x4 = 0.5 * (_x1 + _x2);
            auto x5 = 0.5 * (_x2 + _x0);

            // get the shape functions
            constexpr auto phi_0 = shape_functions.shape<0>();
            constexpr auto phi_1 = shape_functions.shape<1>();
            constexpr auto phi_2 = shape_functions.shape<2>();
            constexpr auto phi_3 = shape_functions.shape<3>();
            constexpr auto phi_4 = shape_functions.shape<4>();
            constexpr auto phi_5 = shape_functions.shape<5>();

            // return the isoparametric mapping from barycentric to physical coordinates
            return _x0 * phi_0 + _x1 * phi_1 + _x2 * phi_2 + x3 * phi_3 + x4 * phi_4 + x5 * phi_5;
        }

      public:
        // get the discretization nodes
        constexpr auto connectivity() const noexcept -> const connectivity_type &
        {
            return _connectivity;
        }

        // get the isoparametric mapping from barycentric coordinates to physical coordinates
        constexpr auto parametrization() const
        {
            // assemble the physical coordinates from the barycentric coordinates
            auto x_cell = functions::function(
                [&](const barycentric_coordinates_type & xi) -> tensor::vector_t<2> {
                    // compute the gradient of the isoparametric mapping
                    return _x_cell()({ xi[0], xi[1] });
                });

            // and return it
            return x_cell;
        }

        // get the shape function associated with local node {a}
        template <int a>
        requires(a >= 0 && a < n_nodes)
        constexpr auto shape() const
        {
            // assemble the shape function associated with local node {a} as a function of
            // barycentric coordinates
            constexpr auto shape_function = functions::function(
                [](const barycentric_coordinates_type & xi) -> tensor::scalar_t {
                    // return the a-th shape function evaluated at {xi}
                    return shape_functions.shape<a>()({ xi[0], xi[1] });
                });

            // and return it
            return shape_function;
        }

        // get the jacobian of the isoparametric mapping from barycentric to actual coordinates
        constexpr auto jacobian() const
        {
            // assemble the jacobian as a function of barycentric coordinates
            auto jacobian_function = functions::function(
                [&](const barycentric_coordinates_type & xi) -> tensor::matrix_t<2> {
                    auto x3 = 0.5 * (_x0 + _x1);
                    auto x4 = 0.5 * (_x1 + _x2);
                    auto x5 = 0.5 * (_x2 + _x0);

                    // get the shape functions derivatives
                    constexpr auto dphi_0 = shape_functions.dshape<0>();
                    constexpr auto dphi_1 = shape_functions.dshape<1>();
                    constexpr auto dphi_2 = shape_functions.dshape<2>();
                    constexpr auto dphi_3 = shape_functions.dshape<3>();
                    constexpr auto dphi_4 = shape_functions.dshape<4>();
                    constexpr auto dphi_5 = shape_functions.dshape<5>();

                    // compute the gradient of the isoparametric mapping
                    return (
                        tensor::dyadic(_x0, dphi_0({ xi[0], xi[1] }))
                        + tensor::dyadic(_x1, dphi_1({ xi[0], xi[1] }))
                        + tensor::dyadic(_x2, dphi_2({ xi[0], xi[1] }))
                        + tensor::dyadic(x3, dphi_3({ xi[0], xi[1] }))
                        + tensor::dyadic(x4, dphi_4({ xi[0], xi[1] }))
                        + tensor::dyadic(x5, dphi_5({ xi[0], xi[1] })));
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
                [&](const barycentric_coordinates_type & xi) -> tensor::vector_t<2> {
                    // the jacobian of the mapping from the reference element to the physical
                    // element evaluated at {xi}
                    auto J = jacobian()(xi);
                    // the derivative of the coordinates with respect to the barycentric coordinates
                    auto J_inv = tensor::inverse(J);
                    // return the spatial gradients of the shape functions evaluated at {xi}
                    return shape_functions.dshape<a>()({ xi[0], xi[1] }) * J_inv;
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
