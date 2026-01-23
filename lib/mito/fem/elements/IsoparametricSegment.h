// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES
// Class {IsoparametricSegment} represents a first order simplex (segment) equipped with parametric
// coordinates.


namespace mito::fem {

    class IsoparametricSegment : public utilities::Invalidatable {
      public:
        // the dimension of the physical space
        static constexpr int dim = 1;
        // the discretization node type
        using discretization_node_type = discrete::discretization_node_t;
        // the underlying cell type
        using cell_type = geometry::segment_t<dim>;

      protected:
        // cartesian coordinates in 1D
        using coordinates_type = geometry::coordinates_t<1, geometry::CARTESIAN>;
        // the coordinate system type
        using coordinate_system_type = geometry::coordinate_system_t<coordinates_type>;
        // the vector type
        using vector_type = tensor::vector_t<1>;

      public:
        // the default constructor
        constexpr IsoparametricSegment(
            const cell_type & cell, const coordinate_system_type & coord_system) :
            _cell(cell),
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

        // the coordinates of the discretization nodes of the segment
        const vector_type _x0;
        const vector_type _x1;
    };

}    // namespace mito


// end of file
