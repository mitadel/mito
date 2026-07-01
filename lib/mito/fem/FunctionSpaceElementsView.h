// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <class functionSpaceT>
    class FunctionSpaceElementsView {

      private:
        // my template parameter
        using function_space_type = functionSpaceT;
        // the type of the cells of the underlying mesh
        using mesh_cells_type = typename function_space_type::manifold_type::mesh_type::cells_type;
        // the type of the iterator over the cells of the underlying mesh
        using mesh_cells_iterator_type = decltype(std::begin(std::declval<mesh_cells_type>()));

      public:
        // constructor from a manifold
        constexpr FunctionSpaceElementsView(const function_space_type & function_space) :
            _function_space(function_space)
        {}

        // iterator class for the manifold elements view
        class iterator {
          public:
            constexpr iterator(
                const function_space_type & function_space, mesh_cells_iterator_type cell_iter) :
                _function_space(function_space),
                _cell_iterator(cell_iter)
            {}

            constexpr auto operator*() const { return _function_space.element(*_cell_iterator); }

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
            const function_space_type & _function_space;
            mesh_cells_iterator_type _cell_iterator;
        };

        constexpr auto begin() const
        {
            return iterator{ _function_space,
                             std::begin(_function_space.manifold().mesh().cells()) };
        }

        constexpr auto end() const
        {
            return iterator{ _function_space, std::end(_function_space.manifold().mesh().cells()) };
        }

      private:
        // the function space whose elements I am viewing
        const function_space_type & _function_space;
    };

}    // namespace mito


// end of file
