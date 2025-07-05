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
        // the number of discretization discretization nodes
        static constexpr int n_nodes = 3;
        // a collection of discretization discretization nodes
        using discretization_nodes_type = std::array<discretization_node_type, n_nodes>;
        // type of a point in barycentric coordinates
        using barycentric_coordinates_type =
            typename geometric_simplex_type::barycentric_coordinates_type;

      private:
        // linear shape functions on the triangle
        static constexpr auto phi_0 = xi_0;
        static constexpr auto phi_1 = xi_1;
        static constexpr auto phi_2 = xi_2;

        // the shape functions
        static constexpr auto phi = std::make_tuple(phi_0, phi_1, phi_2);

        // the gradients of the shape functions
        static constexpr auto dphi = std::make_tuple(
            fields::gradient(phi_0), fields::gradient(phi_1), fields::gradient(phi_2));

      public:
        // the default constructor
        inline IsoparametricTriangleP1(
            const geometric_simplex_type & geometric_simplex,
            const coordinate_system_type & coord_system,
            const discretization_nodes_type & discretization_nodes) :
            IsoparametricTriangle(geometric_simplex, coord_system),
            _discretization_nodes(discretization_nodes)
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
        inline auto discretization_nodes() const noexcept -> const discretization_nodes_type &
        {
            return _discretization_nodes;
        }

        // get the shape function associated with local node {a}
        template <int a>
        requires(a >= 0 && a < n_nodes)
        inline auto shape() const
        {
            // assemble the shape function associated with local node {a} as a function of
            // barycentric coordinates
            auto shape_function = [](const barycentric_coordinates_type & xi) -> tensor::scalar_t {
                // strip the third barycentric coordinate
                auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };
                // return the a-th shape function evaluated at {xi}
                return std::get<a>(phi)(xi_p);
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
                // assemble the isoparametric mapping from the barycentric coordinates to the actual
                // coordinates on the cell {cell}
                auto x_cell = _x0 * phi_0 + _x1 * phi_1 + _x2 * phi_2;
                // strip the third barycentric coordinate
                auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };
                // compute the gradient of the isoparametric mapping
                return fields::gradient(x_cell)(xi_p);
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
                // strip the third barycentric coordinate
                auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };
                // return the spatial gradients of the shape functions evaluated at {xi}
                return std::get<a>(dphi)(xi_p) * J_inv;
            };
            // and return it
            return gradient_function;
        }

      private:
        // the discretization nodes of the simplex
        const discretization_nodes_type _discretization_nodes;
    };

}    // namespace mito


// end of file
