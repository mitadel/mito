// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_elements_NodalField_h)
#define mito_elements_NodalField_h


namespace mito::fem {

    // TOFIX: order of template parameters
    template <typename T, int D>
    class NodalField {
      public:
        NodalField(int nodes, std::string name = "") :
            _nodes(nodes),
            _name(name),
            _nodalField(_nodes * D, 0.0)
        {}

        ~NodalField() {}

        /**
         * Operator()
         */
        const T & operator()(const int a, const int i) const
        {
            assert(i < D);
            return _nodalField[D * a + i];
        }

        T & operator()(const int a, const int i)
        {
            assert(i < D);
            return _nodalField[D * a + i];
        }

        /**
         * Iterators
         */
        typename std::vector<T>::iterator begin() { return std::begin(_nodalField); }

        typename std::vector<T>::iterator end() { return std::end(_nodalField); }

        typename std::vector<T>::const_iterator begin() const { return std::cbegin(_nodalField); }

        typename std::vector<T>::const_iterator end() const { return std::cend(_nodalField); }

        /**
         * Accessors
         */
        inline const std::string & name() const noexcept { return _name; }

        inline int dim() const noexcept { return D; }

        inline int nodes() const noexcept { return _nodes; }

        inline int size() const noexcept { return _nodes * D; }

        /**
         * Set the field to zero.
         */
        inline void init() { std::ranges::fill(_nodalField, 0.0); }

      private:
        /**
         * number of nodes
         */
        int _nodes;

        /**
         * name of the nodal field
         */
        std::string _name;

        /**
         * nodal field
         */
        std::vector<T> _nodalField;
    };

    template <typename T, int D>
    std::ostream & operator<<(std::ostream & os, const NodalField<T, D> & nodalField)
    {

        os << "Nodal field \"" << nodalField.name() << "\" : ";

        if (std::size(nodalField) == 0) {
            os << "[]";
            return os;
        }

        os << "[(" << nodalField(0, 0);
        for (int d = 1; d < D; ++d) {
            os << ", " << nodalField(0, d);
        }
        os << ")";

        for (auto i = 1; i < nodalField.nodes(); ++i) {
            os << ", (" << nodalField(i, 0);
            for (int d = 1; d < D; ++d) {
                os << ", " << nodalField(i, d);
            }
            os << ")";
        }

        os << "]";

        return os;
    }

}    // namespace mito

#endif    // mito_elements_NodalField_h

// end of file
