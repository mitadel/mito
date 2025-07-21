// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangleP1} represents a second order simplex living in 2D cartesian space,
// equipped with linear shape functions defined in the parametric space.


namespace mito::discretization {

    class IsoparametricTriangleP1 : public IsoparametricTriangle {

      public:
        // the degree of the finite element
        static constexpr int degree = 1;
        // the type of shape functions
        using shape_functions_type = ShapeTriangleP1;
        // the linear shape functions
        static constexpr auto shape_functions = shape_functions_type();
        // the number of discretization discretization nodes
        static constexpr int n_nodes = shape_functions_type::N;
        // a collection of discretization discretization nodes
        using connectivity_type = std::array<discretization_node_type, n_nodes>;

      public:
        // the default constructor
        inline IsoparametricTriangleP1(
            const geometric_simplex_type & geometric_simplex,
            const coordinate_system_type & coord_system, const connectivity_type & connectivity) :
            IsoparametricTriangle(geometric_simplex, coord_system),
            _connectivity(connectivity)
        {}

        // destructor
        inline ~IsoparametricTriangleP1() = default;

        // delete move constructor
        inline IsoparametricTriangleP1(IsoparametricTriangleP1 &&) noexcept = delete;

        // delete copy constructor
        inline IsoparametricTriangleP1(const IsoparametricTriangleP1 &) = delete;

        // delete assignment operator
        inline IsoparametricTriangleP1 & operator=(const IsoparametricTriangleP1 &) = delete;

        // delete move assignment operator
        inline IsoparametricTriangleP1 & operator=(IsoparametricTriangleP1 &&) noexcept = delete;

      public:
        // get the discretization nodes
        inline auto connectivity() const noexcept -> const connectivity_type &
        {
            return _connectivity;
        }

        // get the shape function associated with local node {a}
        template <int a>
        requires(a >= 0 && a < n_nodes)
        inline auto shape() const
        {
            // assemble the shape function associated with local node {a} as a function of
            // barycentric coordinates
            auto shape_function = [](const barycentric_coordinates_type & xi) -> tensor::scalar_t {
                // return the a-th shape function evaluated at {xi}
                return shape_functions.shape<a>()({ xi[0], xi[1] });
            };

            // and return it
            return shape_function;
        }

        // get the jacobian of the isoparametric mapping from barycentric to actual coordinates
        inline auto jacobian() const
        {
            // assemble the jacobian as a function of barycentric coordinates
            auto jacobian_function =
                [&](const barycentric_coordinates_type & xi) -> tensor::matrix_t<2> {
                // get the shape functions
                auto phi_0 = shape_functions.shape<0>();
                auto phi_1 = shape_functions.shape<1>();
                auto phi_2 = shape_functions.shape<2>();
                // assemble the isoparametric mapping from the barycentric coordinates to the actual
                // coordinates on the cell {cell}
                auto x_cell = _x0 * phi_0 + _x1 * phi_1 + _x2 * phi_2;
                // compute the gradient of the isoparametric mapping
                return fields::gradient(x_cell)({ xi[0], xi[1] });
            };

            // and return it
            return jacobian_function;
        }

        // get the gradient of the a-th shape function as a function of barycentric coordinates
        template <int a>
        requires(a >= 0 && a < n_nodes)
        inline auto gradient() const
        {
            // assemble the gradient as a function of barycentric coordinates
            auto gradient_function =
                [&](const barycentric_coordinates_type & xi) -> tensor::vector_t<2> {
                // the jacobian of the mapping from the reference element to the physical element
                // evaluated at {xi}
                auto J = jacobian()(xi);
                // the derivative of the coordinates with respect to the barycentric coordinates
                auto J_inv = tensor::inverse(J);
                // return the spatial gradients of the shape functions evaluated at {xi}
                return shape_functions.dshape<a>()({ xi[0], xi[1] }) * J_inv;
            };
            // and return it
            return gradient_function;
        }

      private:
        // the discretization nodes of the simplex
        const connectivity_type _connectivity;
    };

}    // namespace mito


// end of file
