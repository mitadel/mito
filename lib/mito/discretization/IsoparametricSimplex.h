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
        // TOFIX: this should be defined based on the order of the simplex (e.g. 1 for segments, 2
        // for triangles, 3 for tetrahedra, etc.)
        // the type of coordinates
        using parametric_coordinates_type =
            mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

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

      public:
        // get the I-th shape function
        template <int I>
        constexpr auto shape() const
        {
            // return the I-th shape function
            return std::get<I>(phi);
        }

        // get the gradient of the I-th shape function
        template <int I>
        constexpr auto gradient() const
        {
            // return the gradient of I-th shape function
            return std::get<I>(dphi);
        }

        // get the the isoparametric mapping from barycentric to actual coordinates
        template <class coordinateSystemT>
        constexpr auto isoparametric_mapping(
            const geometric_simplex_type & simplex,
            const coordinateSystemT & coordinate_system) const
        {
            // the nodes of the simplex
            const auto & nodes = simplex.nodes();

            // the origin of the coordinate system
            auto origin = typename coordinateSystemT::coordinates_type{};

            // the coordinates of the nodes of the triangle
            auto x_0 = coordinate_system.coordinates(nodes[0]->point()) - origin;
            auto x_1 = coordinate_system.coordinates(nodes[1]->point()) - origin;
            auto x_2 = coordinate_system.coordinates(nodes[2]->point()) - origin;

            // assemble the isoparametric mapping from the barycentric coordinates to the actual
            // coordinates on the cell {cell}
            auto x_cell = x_0 * phi_0 + x_1 * phi_1 + x_2 * phi_2;

            // return the isoparametric mapping
            return x_cell;
        }
    };

}    // namespace mito


// end of file
