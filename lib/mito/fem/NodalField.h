// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <class Y>
    class NodalField {

      private:
        // conventionally packed grid for {n}
        using pack_type = pyre::grid::canonical_t<1>;
        // of Y on the heap
        using storage_type = pyre::memory::heap_t<Y>;
        // putting it all together
        using grid_type = pyre::grid::grid_t<pack_type, storage_type>;
        // index
        using index_type = pack_type::index_type;

      public:
        // constructor
        NodalField(int nodes, std::string name) : NodalField(pack_type{ { nodes } }, name) {}

        // destructor
        ~NodalField() = default;

      private:
        inline NodalField(const pack_type && packing, std::string name) :
            _grid{ packing, packing.cells() },
            _name(name)
        {}

      public:
        /**
         * Operator()
         */
        inline auto operator()(const int a) const -> const Y & { return _grid[a]; }

        inline auto operator()(const int a) -> Y & { return _grid[a]; }

        /**
         * accessor for the number of nodes
         */
        inline auto n_nodes() const -> int { return _grid.layout().shape()[0]; }

        /**
         * accessor for name
         */
        inline const std::string & name() const noexcept { return _name; }

        // support for ranged for loops (wrapping grid)
        inline auto begin() const { return std::cbegin(_grid); }
        inline auto end() const { return std::cend(_grid); }
        inline auto begin() { return std::begin(_grid); }
        inline auto end() { return std::end(_grid); }

      private:
        // the underlying grid
        grid_type _grid;

        // the name of the field
        std::string _name;
    };

    template <class Y>
    std::ostream & operator<<(std::ostream & os, const NodalField<Y> & nodalField)
    {

        os << "Nodal field \"" << nodalField.name() << "\" : " << std::endl;


        for (auto i = 0; i < nodalField.n_nodes(); ++i) {
            os << "Node " << i << ": " << nodalField(i) << std::endl;
        }

        return os;
    }

}    // namespace mito


// end of file
