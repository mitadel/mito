// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricTriangle} represents a second order simplex equipped with barycentric
// coordinates. Template parameters:
//   - coordsT: The coordinate type (determines ambient dimension D and coordinate system)
//   - VolumeFormT: The type of the volume form (N-form for integration)
// This design supports triangles in any coordinate system and any
// embedding (N=D for non-embedded, N<D for embedded manifolds).


namespace mito::fem {

    template <geometry::coordinates_c coordsT, class VolumeFormT>
    class IsoparametricTriangle : public utilities::Invalidatable {
      public:
        // dimension of the parametric space
        static constexpr int N = 2;
        // ambient dimension from coordinate type
        static constexpr int D = coordsT::dim;

        // the Jacobian is a D×N matrix (for a triangle: D×2)
        using jacobian_type = tensor::matrix_t<D, N>;

        // the discretization node type
        using discretization_node_type = discrete::discretization_node_t;
        // the underlying cell type
        using cell_type = geometry::triangle_t<D>;

      protected:
        // coordinate types
        using coordinates_type = coordsT;
        using coordinate_system_type = geometry::coordinate_system_t<coordinates_type>;
        // the vector type
        using vector_type = tensor::vector_t<D>;
        // volume form type
        using volume_form_type = VolumeFormT;

      public:
        // the default constructor
        constexpr IsoparametricTriangle(
            const cell_type & cell, const coordinate_system_type & coord_system,
            const volume_form_type & volume_form) :
            _cell(cell),
            _coord_system(coord_system),
            _volume_form(volume_form),
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

        // the volume form (received from manifold/discretizer)
        const volume_form_type & _volume_form;

        // the coordinates of the discretization nodes of the triangle
        const vector_type _x0;
        const vector_type _x1;
        const vector_type _x2;
    };

}    // namespace mito


// end of file
