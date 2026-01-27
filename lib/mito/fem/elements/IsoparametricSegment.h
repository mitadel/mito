// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricSegment} represents a first order simplex (segment) equipped with parametric
// coordinates. Template parameters:
//   - coordsT: The coordinate type (determines ambient dimension D and coordinate system)
//   - VolumeFormT: The type of the volume form (N-form for integration)
// This design supports segments in any coordinate system and any embedding (N=D
// for non-embedded, N<D for embedded manifolds).


namespace mito::fem {

    template <geometry::coordinates_c coordsT, class VolumeFormT>
    class IsoparametricSegment : public utilities::Invalidatable {
      public:
        // dimension of the parametric space
        static constexpr int N = 1;
        // ambient dimension from coordinate type
        static constexpr int D = coordsT::dim;

        // the Jacobian is a D×N matrix (for a segment: D×1, a
        // column vector)
        using jacobian_type = tensor::matrix_t<D, N>;

        // the discretization node type
        using discretization_node_type = discrete::discretization_node_t;
        // the underlying cell type
        using cell_type = geometry::segment_t<D>;

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
        constexpr IsoparametricSegment(
            const cell_type & cell, const coordinate_system_type & coord_system,
            const volume_form_type & volume_form) :
            _cell(cell),
            _coord_system(coord_system),
            _volume_form(volume_form),
            _x0{ coord_system.coordinates(cell.nodes()[0]->point()) - coordinates_type{} },
            _x1{ coord_system.coordinates(cell.nodes()[1]->point()) - coordinates_type{} }
        {}

        // destructor
        constexpr ~IsoparametricSegment() = default;

        // delete move constructor
        constexpr IsoparametricSegment(IsoparametricSegment &&) noexcept = delete;

        // delete copy constructor
        constexpr IsoparametricSegment(const IsoparametricSegment &) = delete;

        // delete assignment operator
        constexpr IsoparametricSegment & operator=(const IsoparametricSegment &) = delete;

        // delete move assignment operator
        constexpr IsoparametricSegment & operator=(IsoparametricSegment &&) noexcept = delete;

      public:
        // get the geometric simplex
        constexpr auto cell() const noexcept -> const cell_type & { return _cell; }

      protected:
        // QUESTION: do we need to maintain a reference to the geometric simplex?
        // a const reference to the geometric simplex
        const cell_type & _cell;

        // a const reference to the coordinate system
        const coordinate_system_type & _coord_system;

        // the volume form (received from manifold/discretizer)
        const volume_form_type & _volume_form;

        // the coordinates of the discretization nodes of the segment
        const vector_type _x0;
        const vector_type _x1;
    };

}    // namespace mito


// end of file
