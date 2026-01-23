// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangle} represents a second order simplex equipped barycentric coordinates.


namespace mito::fem {

    class IsoparametricTriangle : public utilities::Invalidatable {
      public:
        // the dimension of the physical space
        static constexpr int dim = 2;
        // the discretization node type
        using discretization_node_type = discrete::discretization_node_t;
        // the underlying cell type
        using cell_type = geometry::triangle_t<dim>;

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
            const cell_type & cell, const coordinate_system_type & coord_system) :
            _cell(cell),
            _coord_system(coord_system),
            _x0{ coord_system.coordinates(cell.nodes()[0]->point()) - coordinates_type{} },
            _x1{ coord_system.coordinates(cell.nodes()[1]->point()) - coordinates_type{} },
            _x2{ coord_system.coordinates(cell.nodes()[2]->point()) - coordinates_type{} }
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
        constexpr auto cell() const noexcept -> const cell_type & { return _cell; }

        // get the mapping from parametric coordinates to physical coordinates
        constexpr auto parametrization() const { return _cell.parametrization(_coord_system); }

      protected:
        // a const reference to the geometric simplex
        const cell_type & _cell;

        // a const reference to the coordinate system
        const coordinate_system_type & _coord_system;

        // the coordinates of the discretization nodes of the triangle
        const vector_type _x0;
        const vector_type _x1;
        const vector_type _x2;
    };

}    // namespace mito


// end of file
