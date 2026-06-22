// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    template <class cellT, geometry::coordinates_c coordsT>
    // compatible dimension of physical embedding of cell and type of coordinates
    requires(cellT::dim == coordsT::dim)
    class Atlas {
      public:
        // the cell type
        using cell_type = cellT;
        // typedef for a set of coordinates
        using coordinates_type = coordsT;
        // typedef for a coordinates system
        using coordinate_system_type = geometry::coordinate_system_t<coordinates_type>;

      public:
        // the constructor
        constexpr Atlas(const coordinate_system_type & coord_system) : _coord_system(coord_system)
        {}

        // destructor
        constexpr ~Atlas() = default;

        // delete default constructor
        constexpr Atlas() noexcept = delete;

        // default move constructor
        constexpr Atlas(Atlas &&) noexcept = default;

        // delete copy constructor
        constexpr Atlas(const Atlas &) = delete;

        // delete assignment operator
        constexpr Atlas & operator=(const Atlas &) = delete;

        // default move assignment operator
        constexpr Atlas & operator=(Atlas &&) noexcept = default;

      public:
        // accessor to the coordinate system
        constexpr auto coordinate_system() const -> const coordinate_system_type &
        {
            return _coord_system;
        }

        // return the parametrization of a cell in physical space
        constexpr auto parametrization(const cell_type & cell) const -> auto
        {
            // helper to assemble the parametrization on this cell
            constexpr auto _assemble = []<int... a>(
                                           const auto & cell, const auto & coord_system,
                                           tensor::integer_sequence<a...>) {
                // get the origin of the coordinate system
                auto origin = coord_system.origin();
                // assemble the parametrization as x0 * xi<0> + ...
                // where {xi<a>} are the parametric coordinates on the reference simplex and the
                // {xa} are the position vectors of the nodes
                return (
                    ((cell.template xi<a>()
                      * (coord_system.coordinates(cell.nodes()[a]->point()) - origin)))
                    + ...);
            };
            return _assemble(
                cell, _coord_system, tensor::make_integer_sequence<cell_type::n_vertices>{});
        }

      private:
        // a const reference to the coordinate system
        const coordinate_system_type & _coord_system;
    };

}    // namespace mito


// end of file
