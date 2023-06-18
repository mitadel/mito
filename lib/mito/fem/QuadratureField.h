// code guard
#if !defined(mito_fem_QuadratureField_h)
#define mito_fem_QuadratureField_h


namespace mito::fem {

    template <int Q, class Y>
    class QuadratureField {

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
        inline QuadratureField(int /*nElements*/, const pack_t & packing, std::string name = "") :
            _grid { packing, packing.cells() },
            _name(name)
        {}

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
         * accessor for the number of elements
         * @return the number of elements
         */
        inline int n_elements() const { return _grid.layout().shape()[0]; }

        /*
         * accessor for the number of quadrature points per element
         * @return the number of quadrature point per element
         */
        inline constexpr int n_quad_points() const { return Q; }

        /**
         * const accessor for name
         */
        inline std::string name() const { return _name; }

        /**
         * setter method for name
         */
        inline void name(std::string name)
        {
            _name = name;
            return;
        }

        // support for ranged for loops (wrapping grid)
        inline auto begin() const { return std::cbegin(_grid); }
        inline auto end() const { return std::cend(_grid); }
        inline auto begin() { return std::begin(_grid); }
        inline auto end() { return std::end(_grid); }

      private:
        // instantiate the grid
        grid_t _grid;

        // name of the field
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

#endif    // mito_fem_QuadratureField_h

// end of file
