// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <int Q, class Y>
    class QuadratureField {

      private:
        // conventionally packed grid for {e, q}
        using pack_type = pyre::grid::canonical_t<2>;
        // of Y on the heap
        using storage_type = pyre::memory::heap_t<Y>;
        // putting it all together
        using grid_type = pyre::grid::grid_t<pack_type, storage_type>;
        // index
        using index_type = pack_type::index_type;

      public:
        /**
         * constructor
         * @param[in] elements number of elements for which data are stored
         */
        inline QuadratureField(int nElements, std::string name) :
            QuadratureField(pack_type{ { nElements, Q } }, name)
        {}

      private:
        inline QuadratureField(const pack_type && packing, std::string name) :
            _grid{ packing, packing.cells() },
            _name(name)
        {}

      public:
        // destructor
        ~QuadratureField() = default;

      public:
        /**
         * mutator to an array of data stored at a quadrature point of an element
         * @param[in] e index of the element
         * @param[in] q local index of the quadrature point in the element
         * @return the data
         */
        inline auto operator()(int e, int q) -> Y &
        {
            // all done
            return operator[]({ e, q });
        }

        /**
         * accessor to an array of data stored at a quadrature point of an element
         * @param[in] e index of the element
         * @param[in] q local index of the quadrature point in the element
         * @return the data
         */
        inline auto operator()(int e, int q) const -> const Y &
        {
            // all done
            return operator[]({ e, q });
        }

        inline auto operator[](const index_type & index) -> Y &
        {
            // all done
            return _grid[index];
        }

        inline auto operator[](const index_type & index) const -> const Y &
        {
            // all done
            return _grid[index];
        }

        /**
         * accessor for the number of elements
         * @return the number of elements
         */
        inline auto n_elements() const -> int { return _grid.layout().shape()[0]; }

        /*
         * accessor for the number of quadrature points per element
         * @return the number of quadrature point per element
         */
        constexpr auto n_quad_points() const noexcept -> int { return Q; }

        /**
         * accessor for name
         */
        inline auto name() const noexcept -> std::string { return _name; }

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

    template <int Q, class Y>
    std::ostream & operator<<(std::ostream & os, const quadrature_field_t<Q, Y> & quadratureField)
    {
        os << "Quadrature field \"" << quadratureField.name() << "\" : ";

        for (int e = 0; e < quadratureField.n_elements(); ++e) {
            os << "Element e = " << e << std::endl;
            for (int q = 0; q < quadratureField.n_quad_points(); ++q) {
                os << "\tQuadrature point q = " << q << std::endl;
                os << "\t\t" << quadratureField[{ e, q }] << std::endl;
            }
        }

        return os;
    }

}    // namespace mito


// end of file
