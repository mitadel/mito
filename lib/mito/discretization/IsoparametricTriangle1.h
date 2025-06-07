// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangle1} represents a second order simplex equipped with linear shape
// functions defined in the parametric space.


namespace mito::discretization {

    // QUESTION: should we enforce the use of a cartesian coordinate system for this type of
    // isoparametric simplex? In this case, the coordinate system type can be sinthesized from the
    // geometric simplex type (which has knowledge of the dimension of the embedding space)
    template <
        geometry::geometric_simplex_c geometricSimplexT,
        geometry::coordinate_system_c coordinateSystemT>
    class IsoparametricTriangle1 : public IsoparametricTriangle<geometricSimplexT> {

      public:
        // the number of discretization nodes
        static constexpr int n_nodes = 3;
        // the node type
        using node_type = discretization_node_t;
        // a collection of discretization nodes
        using nodes_type = std::array<node_type, n_nodes>;
        // type of a point in barycentric coordinates
        using barycentric_coordinates_type =
            typename geometricSimplexT::barycentric_coordinates_type;

      private:
        // the geometric simplex type
        using geometric_simplex_type = geometricSimplexT;
        // the global coordinate system type
        using coordinate_system_type = coordinateSystemT;
        // the base class
        using isoparametric_triangle_type = IsoparametricTriangle<geometric_simplex_type>;
        // the parametric coordinates type
        using parametric_coordinates_type =
            typename isoparametric_triangle_type::parametric_coordinates_type;

      private:
        //
        using vector_type = tensor::vector_t<coordinate_system_type::dim>;
        // TOFIX: the number of entries in the map is known at complie time, so maybe we should pick
        // another data structure
        using evaluated_shape_functions_type = std::map<node_type, mito::tensor::scalar_t>;
        using evaluated_shape_functions_gradients_type =
            std::map<node_type, mito::tensor::vector_t<2>>;

      private:
        // linear shape functions on the triangle
        static constexpr auto phi_0 = isoparametric_triangle_type::xi_0;
        static constexpr auto phi_1 = isoparametric_triangle_type::xi_1;
        static constexpr auto phi_2 = isoparametric_triangle_type::xi_2;

        // the shape functions
        static constexpr auto phi = std::make_tuple(phi_0, phi_1, phi_2);

        // the gradients of the shape functions
        static constexpr auto dphi = std::make_tuple(
            mito::fields::gradient(phi_0), mito::fields::gradient(phi_1),
            mito::fields::gradient(phi_2));

      public:
        // the default constructor
        constexpr IsoparametricTriangle1(
            const geometric_simplex_type & geometric_simplex, const nodes_type & nodes,
            const vector_type & x0, const vector_type & x1, const vector_type & x2) :
            IsoparametricTriangle<geometric_simplex_type>(geometric_simplex),
            _nodes(nodes),
            _x0(x0),
            _x1(x1),
            _x2(x2)
        {}

        // destructor
        constexpr ~IsoparametricTriangle1() = default;

        // delete move constructor
        constexpr IsoparametricTriangle1(IsoparametricTriangle1 &&) noexcept = delete;

        // delete copy constructor
        constexpr IsoparametricTriangle1(const IsoparametricTriangle1 &) = delete;

        // delete assignment operator
        constexpr IsoparametricTriangle1 & operator=(const IsoparametricTriangle1 &) = delete;

        // delete move assignment operator
        constexpr IsoparametricTriangle1 & operator=(IsoparametricTriangle1 &&) noexcept = delete;

      public:
        // get the nodes
        constexpr auto nodes() const noexcept -> const nodes_type & { return _nodes; }

        // get all the shape functions evaluated at the point {xi} in barycentric coordinates
        auto shape(const barycentric_coordinates_type & xi) const -> evaluated_shape_functions_type
        {
            // the parametric coordinates of the quadrature point
            auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };

            // return the shape functions evaluated at {xi}
            return { { _nodes[0], std::get<0>(phi)(xi_p) },
                     { _nodes[1], std::get<1>(phi)(xi_p) },
                     { _nodes[2], std::get<2>(phi)(xi_p) } };
        }

        // get the jacobian of the isoparametric mapping from barycentric to actual coordinates
        constexpr auto jacobian(const barycentric_coordinates_type & xi) const
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
        auto gradient(const barycentric_coordinates_type & xi) const
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
            return { { _nodes[0], std::get<0>(dphi)(xi_p) * J_inv },
                     { _nodes[1], std::get<1>(dphi)(xi_p) * J_inv },
                     { _nodes[2], std::get<2>(dphi)(xi_p) * J_inv } };
        }

      private:
        // the nodes of the simplex
        const nodes_type _nodes;
        // QUESTION: alternatively to the coordinates of the vertices, we could store the points
        // associated with the vertices of the triangle, so the coordinates can be fetched from the
        // coordinate system
        //
        // the coordinates of the nodes of the triangle
        const vector_type _x0;
        const vector_type _x1;
        const vector_type _x2;
    };

}    // namespace mito


// end of file
