// code guard
#if !defined(mito_elements_QuadratureField_h)
#define mito_elements_QuadratureField_h

#include "../mito.h"
#include <pyre/grid.h>

namespace mito {

    template <int Q, typename Y>
    class QuadratureField {

      public:
        static constexpr int D = size<Y>::value;

      private:
        // conventionally packed grid for {e, q}
        using pack_t = pyre::grid::canonical_t<2>;
        // of Y on the heap
        using storage_t = pyre::memory::heap_t<Y>;
        // putting it all together
        using grid_t = pyre::grid::grid_t<pack_t, storage_t>;
        // index
        using index_t = pack_t::index_type;

      public:
        /**
         * constructor
         * @param[in] elements number of elements for which data are stored
         */
        inline QuadratureField(int nElements, std::string name = "") :
            QuadratureField(nElements, pack_t { { nElements, Q } }, name)
        {}

      private:
        inline QuadratureField(int nElements, const pack_t & packing, std::string name = "") :
            _grid { packing, packing.cells() }, _name(name)
        {
            // initialize memory
            reset();

            // all done
            return;
        }

      public:
        // destructor
        ~QuadratureField() {}

      public:
        /**
         * mutator to an array of data stored at a quadrature point of an element
         * @param[in] e index of the element
         * @param[in] q local index of the quadrature point in the element
         * @return the data
         */
        inline Y & operator()(int e, int q)
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
        inline const Y & operator()(int e, int q) const
        {
            // all done
            return operator[]({ e, q });
        }

        inline Y & operator[](const index_t & index)
        {
            // all done
            return _grid[index];
        }

        inline const Y & operator[](const index_t & index) const
        {
            // all done
            return _grid[index];
        }

        /**
         * accessor for the size of array stored per quadrature point per element
         * @return the size of array stored per quadrature point per element
         */
        inline constexpr int n_dim() { return D; }

        /**
         * accessor for the number of elements
         * @return the number of elements
         */
        inline int n_elements() const { return _grid.layout().shape()[0]; }

        /*
         * accessor for the quadrature points per element
         * @return the number of quadrature point per element
         */
        inline constexpr int n_quad() { return Q; }

        /**
         * reset to zero
         */
        inline void reset()
        {
            // reset all entries
            for (const auto & idx : _grid.layout()) {
                _grid[idx].reset();
            }
            // all done
            return;
        }

        /**
         * const accessor for name
         */
        inline std::string name() const { return _name; }

        /**
         * non-const accessor for name
         */
        inline std::string& name() { return _name; }

        // support for ranged for loops (wrapping grid)
        inline const auto begin() const { return _grid.cbegin(); }
        inline const auto end() const { return _grid.cend(); }
        inline auto begin() { return _grid.begin(); }
        inline auto end() { return _grid.end(); }

      private:
        // instantiate the grid
        grid_t _grid;

        // name of the field
        std::string _name;
    };

    template <int Q, typename Y>
    using quadrature_field_t = QuadratureField<Q, Y>;

}    // namespace mito

#endif    // mito_elements_QuadratureField_h

// end of file
