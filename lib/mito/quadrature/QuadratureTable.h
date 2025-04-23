// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::quadrature {

    // a table to store quadrature rules in terms of their point-weight pairings
    template <class elementT, int Q>
    class Table {

      public:
        // the type of quadrature points' parametric coordinates
        using quadrature_point_type =
            typename elementT::resource_type::barycentric_coordinates_type;
        // the type of quadrature weights
        using quadrature_weight_type = double;
        // the type of quadrature point-weight pairing
        using point_weight_pair_type = std::tuple<quadrature_point_type, quadrature_weight_type>;
        // the type for a collection of such pairings
        using array_type = std::array<point_weight_pair_type, Q>;

      public:
        // constructor
        constexpr Table(array_type && pairs) noexcept : _table{ std::move(pairs) } {}

        // the number of entries in the table (i.e. the pairings in the table)
        constexpr auto size() const -> int { return Q; }

        // get the q-th quadrature point
        constexpr auto point(int q) const -> quadrature_point_type
        {
            return std::get<0>(_table[q]);
        }

        // get the q-th quadrature weight
        constexpr auto weight(int q) const -> quadrature_weight_type
        {
            return std::get<1>(_table[q]);
        }

      private:
        // the table with the quadrature point-weight pairings
        const array_type _table;
    };

    // a factory for quadrature tables
    template <quadrature_formula quadratureT, class elementT, int r>
    constexpr auto QuadratureTable() -> auto;

}    // namespace mito


#include "quadrature_tables_segments.icc"
#include "quadrature_tables_triangles.icc"
#include "quadrature_tables_tetrahedra.icc"


// end of file
