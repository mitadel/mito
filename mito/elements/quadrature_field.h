#include "../mito.h"

#ifndef __MITO__QUADRATURE_FIELD__
#define __MITO__QUADRATURE_FIELD__

namespace mito {

    template <int Q, typename Y>
    class QuadratureField : public std::vector<Y> {

      public:
        using T = typename type<Y>::value;
        static constexpr int D = size<Y>::value;

      public:
        /**
         * constructor
         * @param[in] elements number of elements for which data are stored
         */
        inline QuadratureField(int nElements) : std::vector<Y>(nElements * Q), _nElements(nElements)
        {}

        // destructor
        ~QuadratureField() {}

      public:
        /**
         * mutator to an array of data stored at a quadrature point of an element
         * @param[in] e index of the element
         * @param[in] q local index of the quadrature point in the element
         * @return a pointer to the data
         */
        inline Y & operator()(int e, int q) { return (*this)[e * Q + q]; }

        /**
         * accessor to an array of data stored at a quadrature point of an element
         * @param[in] e index of the element
         * @param[in] q local index of the quadrature point in the element
         * @return a const pointer to the first value of the array of data
         */
        inline const Y & operator()(int e, int q) const { return (*this)[e * Q + q]; }

        /**
         * accessor for the size of array stored per quadrature point per element
         * @return the size of array stored per quadrature point per element
         */
        inline constexpr int n_dim() { return D; }

        /**
         * accessor for the number of elements
         * @return the number of elements
         */
        inline int n_elements() const { return _nElements; }

        /*
         * accessor for the quadrature points per element
         * @return the number of quadrature point per element
         */
        inline constexpr int n_quad() { return Q; }

        /**
         * reset all entries to zero
         */
        inline void reinit()
        {
            // TOFIX
            std::fill(this->begin(), this->end(), 0.0);
            return;
        }

        /**
         * accessor for name
         */
        inline std::string name() const { return _name; }

      private:
        // number of elements
        int _nElements;

        // name of the field
        std::string _name;
    };

    template <int Q, typename Y>
    using quadrature_field_t = QuadratureField<Q, Y>;

}    // namespace mito

#endif    //__MITO__QUADRATURE_FIELD__

// end of file
