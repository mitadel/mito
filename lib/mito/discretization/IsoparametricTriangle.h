// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangle} represents a second order simplex equipped parametric coordinates.


namespace mito::discretization {

    class IsoparametricTriangle : public utilities::Invalidatable {
      public:
        // the discretization node type
        using discretization_node_type = discretization_node_t;
        // the geometric simplex type
        using geometric_simplex_type = geometry::triangle_t<2>;
        // type of a point in barycentric coordinates
        using barycentric_coordinates_type =
            typename geometric_simplex_type::barycentric_coordinates_type;

      protected:
        // cartesian coordinates in 2D
        using coordinates_type = geometry::coordinates_t<2, geometry::CARTESIAN>;
        // the coordinate system type
        using coordinate_system_type = geometry::coordinate_system_t<coordinates_type>;
        // the vector type
        using vector_type = tensor::vector_t<2>;

      public:
        // the default constructor
        constexpr IsoparametricTriangle(
            const geometric_simplex_type & geometric_simplex,
            const coordinate_system_type & coord_system) :
            _geometric_simplex(geometric_simplex),
            _x0{ coord_system.coordinates(geometric_simplex.nodes()[0]->point())
                 - coordinates_type{} },
            _x1{ coord_system.coordinates(geometric_simplex.nodes()[1]->point())
                 - coordinates_type{} },
            _x2{ coord_system.coordinates(geometric_simplex.nodes()[2]->point())
                 - coordinates_type{} }
        {}

        // destructor
        constexpr ~IsoparametricTriangle() = default;

        // delete move constructor
        constexpr IsoparametricTriangle(IsoparametricTriangle &&) noexcept = delete;

        // delete copy constructor
        constexpr IsoparametricTriangle(const IsoparametricTriangle &) = delete;

        // delete assignment operator
        constexpr IsoparametricTriangle & operator=(const IsoparametricTriangle &) = delete;

        // delete move assignment operator
        constexpr IsoparametricTriangle & operator=(IsoparametricTriangle &&) noexcept = delete;

      public:
        // get the geometric simplex
        constexpr auto geometric_simplex() const noexcept -> const geometric_simplex_type &
        {
            return _geometric_simplex;
        }

      protected:
        // QUESTION: do we need to maintain a reference to the geometric simplex?
        // a const reference to the geometric simplex
        const geometric_simplex_type & _geometric_simplex;

        // the coordinates of the discretization nodes of the triangle
        const vector_type _x0;
        const vector_type _x1;
        const vector_type _x2;
    };

}    // namespace mito


// end of file
