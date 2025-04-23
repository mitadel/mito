// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricSimplex} represents the reference simplex in the parametric space

namespace mito::discretization {

    // TODO: add concept of geometric simplex and require that {geomtricSimplexT} is a geometric
    // simplex
    template <class geometricSimplexT>
    class IsoparametricSimplex {

      private:
        // the geometric simplex type
        using geometric_simplex_type = geometricSimplexT;
        // the vector type
        using vector_type = tensor::vector_t<2>;
        // TOFIX: this should be defined based on the order of the simplex (e.g. 1 for segments, 2
        // for triangles, 3 for tetrahedra, etc.)
        // the parametric coordinates type
        using parametric_coordinates_type =
            mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
        // type of a point in barycentric coordinates
        using barycentric_coordinates_type = geometricSimplexT::barycentric_coordinates_type;
        // TOFIX: see above
        using evaluated_shape_functions_type = std::array<mito::tensor::scalar_t, 3>;
        using evaluated_shape_functions_gradients_type = std::array<mito::tensor::vector_t<2>, 3>;

      private:
        // TOFIX: these should be defined based on the order of the element and on the type of
        // simplex (for example here it's 3 of them because we use linear elements on a triangle)
        // the function extracting the 0 component of a parametric point
        static constexpr auto xi_0 = mito::functions::component<parametric_coordinates_type, 0>;
        // the function extracting the y component of a parametric point
        static constexpr auto xi_1 = mito::functions::component<parametric_coordinates_type, 1>;

        // linear shape functions on the triangle
        static constexpr auto phi_0 = mito::fields::field(xi_0);
        static constexpr auto phi_1 = mito::fields::field(xi_1);
        static constexpr auto phi_2 = 1.0 - phi_0 - phi_1;

        // the shape functions
        static constexpr auto phi = std::make_tuple(phi_0, phi_1, phi_2);

        // the gradients of the shape functions
        static constexpr auto dphi = std::make_tuple(
            mito::fields::gradient(phi_0), mito::fields::gradient(phi_1),
            mito::fields::gradient(phi_2));

      public:
        // the default constructor
        constexpr IsoparametricSimplex() = default;

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
        // QUESTION: is there a way to enforce that {barycentricCoordinatesT} are indeed barycentric
        // coordinates
        // get all the shape functions evaluated at the point {xi} in barycentric coordinates
        auto shape(const barycentric_coordinates_type & xi) const -> evaluated_shape_functions_type
        {
            // the parametric coordinates of the quadrature point
            auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };

            // return the shape functions evaluated at {xi}
            return { std::get<0>(phi)(xi_p), std::get<1>(phi)(xi_p), std::get<2>(phi)(xi_p) };
        }

        // get all the shape functions gradients evaluated at the point {xi} in barycentric
        // coordinates
        auto gradient(const barycentric_coordinates_type & xi) const
            -> evaluated_shape_functions_gradients_type
        {
            // the parametric coordinates of the quadrature point
            auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };

            // return the shape functions gradients evaluated at {xi}
            return { std::get<0>(dphi)(xi_p), std::get<1>(dphi)(xi_p), std::get<2>(dphi)(xi_p) };
        }

        // get the jacobian of the isoparametric mapping from barycentric to actual coordinates
        constexpr auto jacobian(
            const vector_type & x_0, const vector_type & x_1, const vector_type & x_2,
            const barycentric_coordinates_type & xi) const
        {
            // assemble the isoparametric mapping from the barycentric coordinates to the actual
            // coordinates on the cell {cell}
            auto x_cell = x_0 * phi_0 + x_1 * phi_1 + x_2 * phi_2;

            // the parametric coordinates of the quadrature point
            auto xi_p = parametric_coordinates_type{ xi[0], xi[1] };

            auto J = mito::fields::gradient(x_cell)(xi_p);

            // return the jacobian of the isoparametric mapping
            return J;
        }
    };

}    // namespace mito


// end of file
