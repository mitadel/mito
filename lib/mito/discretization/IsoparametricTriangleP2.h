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
        // the number of discretization nodes
        static constexpr int n_nodes = 6;
        // a collection of discretization nodes
        using discretization_nodes_type = std::array<discretization_node_type, n_nodes>;
        // type of a point in barycentric coordinates
        using barycentric_coordinates_type =
            typename geometric_simplex_type::barycentric_coordinates_type;

      private:
        // TOFIX: the number of entries in the map is known at complie time, so maybe we should pick
        // another data structure
        using evaluated_shape_functions_type = std::map<discretization_node_type, tensor::scalar_t>;
        using evaluated_shape_functions_gradients_type =
            std::map<discretization_node_type, tensor::vector_t<2>>;

      private:
        // strip the namespace
        static constexpr auto xi_0 = xi_0;
        static constexpr auto xi_1 = xi_1;
        static constexpr auto xi_2 = xi_2;

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
            fields::gradient(phi_0), fields::gradient(phi_1), fields::gradient(phi_2),
            fields::gradient(phi_3), fields::gradient(phi_4), fields::gradient(phi_5));

      public:
        // the default constructor
        inline IsoparametricTriangleP2(
            const geometric_simplex_type & geometric_simplex,
            const coordinate_system_type & coord_system,
            const discretization_nodes_type & discretization_nodes) :
            IsoparametricTriangle(geometric_simplex, coord_system),
            _discretization_nodes(discretization_nodes)
        {}

        // destructor
        inline ~IsoparametricTriangleP2() = default;

        // delete move constructor
        inline IsoparametricTriangleP2(IsoparametricTriangleP2 &&) noexcept = delete;

        // delete copy constructor
        inline IsoparametricTriangleP2(const IsoparametricTriangleP2 &) = delete;

        // delete assignment operator
        inline IsoparametricTriangleP2 & operator=(const IsoparametricTriangleP2 &) = delete;

        // delete move assignment operator
        inline IsoparametricTriangleP2 & operator=(IsoparametricTriangleP2 &&) noexcept = delete;

      public:
        // get the discretization nodes
        inline auto discretization_nodes() const noexcept -> const discretization_nodes_type &
        {
            return _discretization_nodes;
        }

        // get all the shape functions evaluated at the point {xi} in barycentric coordinates
        inline auto shape(const barycentric_coordinates_type & xi) const
            -> evaluated_shape_functions_type
        {
            // the parametric coordinates of the quadrature point
            auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };

            // return the shape functions evaluated at {xi}
            return { { _discretization_nodes[0], std::get<0>(phi)(xi_p) },
                     { _discretization_nodes[1], std::get<1>(phi)(xi_p) },
                     { _discretization_nodes[2], std::get<2>(phi)(xi_p) },
                     { _discretization_nodes[3], std::get<3>(phi)(xi_p) },
                     { _discretization_nodes[4], std::get<4>(phi)(xi_p) },
                     { _discretization_nodes[5], std::get<5>(phi)(xi_p) } };
        }

        // get the jacobian of the isoparametric mapping from barycentric to actual coordinates
        inline auto jacobian(const barycentric_coordinates_type & xi) const
        {
            auto x3 = 0.5 * (_x0 + _x1);
            auto x4 = 0.5 * (_x1 + _x2);
            auto x5 = 0.5 * (_x2 + _x0);

            // assemble the isoparametric mapping from the barycentric coordinates to the actual
            // coordinates on the cell {cell}
            auto x_cell =
                _x0 * phi_0 + _x1 * phi_1 + _x2 * phi_2 + x3 * phi_3 + x4 * phi_4 + x5 * phi_5;

            // the parametric coordinates of the quadrature point
            auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };

            // compute the gradient of the isoparametric mapping
            auto J = fields::gradient(x_cell)(xi_p);

            // return the jacobian of the isoparametric mapping
            return J;
        }

        // get all the shape functions gradients evaluated at the point {xi} in barycentric
        // coordinates
        inline auto gradient(const barycentric_coordinates_type & xi) const
            -> evaluated_shape_functions_gradients_type
        {
            // the jacobian of the mapping from the reference element to the physical element
            // evaluated at {xi}
            auto J = jacobian(xi);

            // the derivative of the coordinates with respect to the barycentric coordinates
            auto J_inv = tensor::inverse(J);

            // the parametric coordinates of the quadrature point
            auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };

            // return the spatial gradients of the shape functions evaluated at {xi}
            return { { _discretization_nodes[0], std::get<0>(dphi)(xi_p) * J_inv },
                     { _discretization_nodes[1], std::get<1>(dphi)(xi_p) * J_inv },
                     { _discretization_nodes[2], std::get<2>(dphi)(xi_p) * J_inv },
                     { _discretization_nodes[3], std::get<3>(dphi)(xi_p) * J_inv },
                     { _discretization_nodes[4], std::get<4>(dphi)(xi_p) * J_inv },
                     { _discretization_nodes[5], std::get<5>(dphi)(xi_p) * J_inv } };
        }

      private:
        // the discretization nodes of the simplex
        const discretization_nodes_type _discretization_nodes;
    };

}    // namespace mito


// end of file
