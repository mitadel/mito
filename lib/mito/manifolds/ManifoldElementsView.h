// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::manifolds {

    template <class manifoldT>
    class ManifoldElementsView {

      private:
        // my template parameter
        using manifold_type = manifoldT;
        // the type of the cells of the underlying mesh
        using mesh_cells_type = typename manifold_type::mesh_type::cells_type;
        // the type of the iterator over the cells of the underlying mesh
        using mesh_cells_iterator_type = decltype(std::begin(std::declval<mesh_cells_type>()));

      public:
        // constructor from a manifold
        constexpr ManifoldElementsView(const manifoldT & manifold) : _manifold(manifold) {}

        // iterator class for the manifold elements view
        class iterator {
          public:
            constexpr iterator(const manifold_type & manifold, mesh_cells_iterator_type cell_iter) :
                _manifold(manifold),
                _cell_iterator(cell_iter)
            {}

            constexpr auto operator*() const { return _manifold.element(*_cell_iterator); }

            constexpr iterator & operator++()
            {
                ++_cell_iterator;
                return *this;
            }

            constexpr bool operator==(const iterator & other) const
            {
                return _cell_iterator == other._cell_iterator;
            }

            constexpr bool operator!=(const iterator & other) const { return !(*this == other); }

          private:
            const manifold_type & _manifold;
            mesh_cells_iterator_type _cell_iterator;
        };

        constexpr auto begin() const
        {
            return iterator{ _manifold, std::begin(_manifold.mesh().cells()) };
        }

        constexpr auto end() const
        {
            return iterator{ _manifold, std::end(_manifold.mesh().cells()) };
        }

      private:
        // the manifold whose elements I am viewing
        const manifold_type & _manifold;
    };

}    // namespace mito


// end of file
