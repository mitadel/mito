// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricSimplex} represents the reference simplex in the parametric space


// TODO: rename to IsoparametricTriangle1. IsoparametricTriangle1 and IsoparametricTriangle2 can be
// derived from a common base class IsoparametricTriangle that has the barycentric coordinates types
// and the isoparametric mapping
namespace mito::discretization {

    template <
        geometry::geometric_simplex_c geometricSimplexT,
        geometry::coordinate_system_c coordinateSystemT>
    class IsoparametricSimplex : public utilities::Invalidatable {

      private:
        // the geometric simplex type
        using geometric_simplex_type = geometricSimplexT;
        // TOFIX: we need to come up with another notion for the discretization node as the
        // {geometry::node_t} implies the presence of a vertex the node type
        using node_type = typename geometric_simplex_type::node_type;
        // the number of discretization nodes
        static constexpr int n_nodes = 3;
        // a collection of discretization nodes
        using nodes_type = std::array<node_type, n_nodes>;
        // the global coordinate system type
        using coordinate_system_type = coordinateSystemT;
        // TOFIX: this should be defined based on the order of the simplex (e.g. 1 for segments, 2
        // for triangles, 3 for tetrahedra, etc.)
        // the parametric dimension
        static constexpr int dim = 2;
        // the parametric coordinates type
        using parametric_coordinates_type =
            mito::geometry::coordinates_t<dim, mito::geometry::CARTESIAN>;
        // type of a point in barycentric coordinates
        using barycentric_coordinates_type = geometric_simplex_type::barycentric_coordinates_type;
        // TOFIX: the number of entries in the map is known at complie time, so maybe we should pick
        // another data structure
        using evaluated_shape_functions_type = std::map<node_type, mito::tensor::scalar_t>;
        using evaluated_shape_functions_gradients_type =
            std::map<node_type, mito::tensor::vector_t<dim>>;

      private:
        // TOFIX: these should be defined based on the order of the element and on the type of
        // simplex (for example here it's 3 of them because we use linear elements on a triangle)
        // the function extracting the 0 component of a parametric point
        static constexpr auto xi_0 =
            mito::fields::field(mito::functions::component<parametric_coordinates_type, 0>);
        // the function extracting the 1 component of a parametric point
        static constexpr auto xi_1 =
            mito::fields::field(mito::functions::component<parametric_coordinates_type, 1>);
        // the function extracting the 2 component of a parametric point
        static constexpr auto xi_2 = 1.0 - xi_0 - xi_1;

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
        constexpr IsoparametricSimplex(
            const geometric_simplex_type & geometric_simplex,
            const coordinate_system_type & coordinate_system) :
            _geometric_simplex(geometric_simplex),
            _coordinate_system(coordinate_system),
            _nodes{ geometric_simplex.nodes() }
        {}

        // destructor
        constexpr ~IsoparametricSimplex() = default;

        // delete move constructor
        constexpr IsoparametricSimplex(IsoparametricSimplex &&) noexcept = delete;

        // delete copy constructor
        constexpr IsoparametricSimplex(const IsoparametricSimplex &) = delete;

        // delete assignment operator
        constexpr IsoparametricSimplex & operator=(const IsoparametricSimplex &) = delete;

        // delete move assignment operator
        constexpr IsoparametricSimplex & operator=(IsoparametricSimplex &&) noexcept = delete;

      public:
        // get the geometric simplex
        constexpr auto geometric_simplex() const noexcept -> const geometric_simplex_type &
        {
            return _geometric_simplex;
        }

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
            // the origin of the coordinate system
            auto origin = typename coordinate_system_type::coordinates_type{};

            // the coordinates of the nodes of the triangle
            auto x_0 = _coordinate_system.coordinates(_nodes[0]->point()) - origin;
            auto x_1 = _coordinate_system.coordinates(_nodes[1]->point()) - origin;
            auto x_2 = _coordinate_system.coordinates(_nodes[2]->point()) - origin;

            // assemble the isoparametric mapping from the barycentric coordinates to the actual
            // coordinates on the cell {cell}
            auto x_cell = x_0 * phi_0 + x_1 * phi_1 + x_2 * phi_2;

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
        // a const reference to the geometric simplex
        const geometric_simplex_type & _geometric_simplex;
        // a const reference to the coordinate system
        const coordinate_system_type & _coordinate_system;
        // the nodes of the simplex
        const nodes_type _nodes;
    };

}    // namespace mito


// end of file
