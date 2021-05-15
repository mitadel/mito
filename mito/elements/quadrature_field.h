#include "../mito.h"

#ifndef __MITO__QUADRATURE_FIELD__
#define __MITO__QUADRATURE_FIELD__

namespace mito {

    template <int Q, dim_t D, class T = real>
    class quadrature_field_t : public std::vector<mito::vector<D, T>> {
      public:
        /**
         * donstructor
         * @param[in] elements number of elements for which data are stored
         */
        inline quadrature_field_t(int nElements) :
            std::vector<mito::vector<D, T>>(nElements * Q),
            _nElements(nElements)
        {}

        // destructor
        ~quadrature_field_t() {}

      public:
        /**
         * mutator to an array of data stored at a quadrature point of an element
         * @param[in] e index of the element
         * @param[in] q local index of the quadrature point in the element
         * @return a pointer to the data
         */
        inline mito::vector<D, T> & operator()(int e, int q) { return (*this)[e * Q + q]; }

        /**
         * accessor to an array of data stored at a quadrature point of an element
         * @param[in] e index of the element
         * @param[in] q local index of the quadrature point in the element
         * @return a const pointer to the first value of the array of data
         */
        inline const mito::vector<D, T> & operator()(int e, int q) const
        {
            return (*this)[e * Q + q];
        }

        /**
         * accessor for the size of array stored per quadrature point per element
         * @return the size of array stored per quadrature point per element
         */
        inline dim_t n_dim() const { return D; }

        /**
         * accessor for the number of elements
         * @return the number of elements
         */
        inline int n_elements() const { return _nElements; }

        /*
         * accessor for the quadrature points per element
         * @return the number of quadrature point per element
         */
        inline int n_quad() const { return Q; }

        /**
         * reset all entries to zero.
         */
        inline void reinit()
        {
            std::fill(this->begin(), this->end(), 0.0);
            return;
        }

      private:
        // number of elements
        int _nElements;
    };

}    // namespace mito

#endif    //__MITO__QUADRATURE_FIELD__

// end of file
