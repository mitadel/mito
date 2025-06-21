// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangle1} represents a second order simplex living in 2D cartesian space,
// equipped with linear shape functions defined in the parametric space.


namespace mito::discretization {

    class IsoparametricTriangle1 : public IsoparametricTriangle {

      public:
        // the number of discretization discretization nodes
        static constexpr int n_nodes = 3;
        // a collection of discretization discretization nodes
        using discretization_nodes_type = std::array<discretization_node_type, n_nodes>;
        // type of a point in barycentric coordinates
        using barycentric_coordinates_type =
            typename geometric_simplex_type::barycentric_coordinates_type;

      private:
        //
        using vector_type = tensor::vector_t<2>;
        // TOFIX: the number of entries in the map is known at complie time, so maybe we should pick
        // another data structure
        using evaluated_shape_functions_type =
            std::map<discretization_node_type, mito::tensor::scalar_t>;
        using evaluated_shape_functions_gradients_type =
            std::map<discretization_node_type, mito::tensor::vector_t<2>>;

      private:
        // linear shape functions on the triangle
        static constexpr auto phi_0 = xi_0;
        static constexpr auto phi_1 = xi_1;
        static constexpr auto phi_2 = xi_2;

        // the shape functions
        static constexpr auto phi = std::make_tuple(phi_0, phi_1, phi_2);

        // the gradients of the shape functions
        static constexpr auto dphi = std::make_tuple(
            mito::fields::gradient(phi_0), mito::fields::gradient(phi_1),
            mito::fields::gradient(phi_2));

      public:
        // the default constructor
        inline IsoparametricTriangle1(
            const geometric_simplex_type & geometric_simplex,
            const discretization_nodes_type & discretization_nodes, const vector_type & x0,
            const vector_type & x1, const vector_type & x2) :
            IsoparametricTriangle(geometric_simplex),
            _discretization_nodes(discretization_nodes),
            _x0(x0),
            _x1(x1),
            _x2(x2)
        {}

        // destructor
        inline ~IsoparametricTriangle1() = default;

        // delete move constructor
        inline IsoparametricTriangle1(IsoparametricTriangle1 &&) noexcept = delete;

        // delete copy constructor
        inline IsoparametricTriangle1(const IsoparametricTriangle1 &) = delete;

        // delete assignment operator
        inline IsoparametricTriangle1 & operator=(const IsoparametricTriangle1 &) = delete;

        // delete move assignment operator
        inline IsoparametricTriangle1 & operator=(IsoparametricTriangle1 &&) noexcept = delete;

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
                     { _discretization_nodes[2], std::get<2>(phi)(xi_p) } };
        }

        // get the jacobian of the isoparametric mapping from barycentric to actual coordinates
        inline auto jacobian(const barycentric_coordinates_type & xi) const
        {
            // assemble the isoparametric mapping from the barycentric coordinates to the actual
            // coordinates on the cell {cell}
            auto x_cell = _x0 * phi_0 + _x1 * phi_1 + _x2 * phi_2;

            // the parametric coordinates of the quadrature point
            auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };

            // compute the gradient of the isoparametric mapping
            auto J = mito::fields::gradient(x_cell)(xi_p);

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
            auto J_inv = mito::tensor::inverse(J);

            // the parametric coordinates of the quadrature point
            auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };

            // return the spatial gradients of the shape functions evaluated at {xi}
            return { { _discretization_nodes[0], std::get<0>(dphi)(xi_p) * J_inv },
                     { _discretization_nodes[1], std::get<1>(dphi)(xi_p) * J_inv },
                     { _discretization_nodes[2], std::get<2>(dphi)(xi_p) * J_inv } };
        }

      private:
        // the discretization nodes of the simplex
        const discretization_nodes_type _discretization_nodes;
        // QUESTION: alternatively to the coordinates of the vertices, we could store the points
        // associated with the vertices of the triangle, so the coordinates can be fetched from the
        // coordinate system
        //
        // the coordinates of the discretization nodes of the triangle
        const vector_type _x0;
        const vector_type _x1;
        const vector_type _x2;
    };

}    // namespace mito


// end of file
