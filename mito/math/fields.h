#ifndef __MITO__FIELDS__
#define __MITO__FIELDS__

#include "../mito.h"
#include "../elements/quadrature_field.h"

namespace mito {

    // f(X,t) with (X \in R^D, t \in R) -> R
    template <dim_t D>
    class Field {

        // typedef for a scalar valued function
        using function_t = function<vector<D>>;

      public:
        // constructors
        Field(function_t f) : _f(f), _Df() {}
        Field(function_t f, std::array<function_t, D> Df) : _f(f), _Df(Df) {}

        // delete default constructor
        Field() = delete;

        // delete assignment operator
        Field & operator=(const Field &) = delete;

        ~Field() {}

        // inline real operator()(const vector<D> & X, const vector<D2> & x, real t) const {
        inline auto operator()(const vector<D> & X) const
        {
            // evaluate _f
            return _f(X);
        }

        inline auto operator()(const std::vector<vector<D>> & X) const
        {
            std::vector<real> values(X.size(), 0.0);
            // evaluate operator() at all elements of X
            for (int i = 0; i < (int) X.size(); ++i) {
                values[i] = operator()(X[i]);
            }
            return values;
        }

        template <int Q>
        inline auto operator()(const quadrature_field_t<Q, D> & X) const
        {
            quadrature_field_t<Q, D> values(X.n_elements());

            // evaluate operator() at all elements of X
            for (int e = 0; e < values.n_elements(); ++e) {
                for (int q = 0; q < Q; ++q) {
                    values(e, q)[0] = operator()(X(e, q));
                }
            }

            // all done
            return values;
        }

      public:
        // accessor for function partial derivatives
        inline const auto & Df(int i) const
        {
            // assert there exists the i-th partial derivative
            assert(i < (int) _Df.size());
            // return the i-th partial derivative
            return _Df[i];
        }

      private:
        // the function
        function_t _f;
        // the derivatives of f with respect to X (position in the reference configuration)
        std::array<function_t, D> _Df;
    };

    // template on vector dimension N, spatial dimension D
    template <dim_t D, dim_t N>
    class VectorField {

      public:
        VectorField(std::array<Field<D>, N> components) : _components(components) {}
        ~VectorField() {}

        // delete default constructor
        VectorField() = delete;

        // delete copy constructor
        VectorField(const VectorField &) = delete;

        // delete assignment operator
        VectorField & operator=(const VectorField &) = delete;

        inline auto operator()(const vector<D> & X) const
        {
            vector<N> result;
            for (int i = 0; i < N; ++i) {
                result[i] = _components[i](X);
            }
            return result;
        }

        template <int Q>
        inline auto operator()(const quadrature_field_t<Q, D> & X) const
        {
            vector<N> result;
            for (int i = 0; i < N; ++i) {
                result[i] = _components[i](X);
            }
            return result;
        }

        inline const auto & operator[](int i) const
        {
            // assert there exists the i-th partial derivative
            assert(i < (int) _components.size());
            // return the i-th component
            return _components[i];
        }

        inline auto & operator[](int i)
        {
            // assert there exists the i-th partial derivative
            assert(i < _components.size());
            // return the i-th component
            return _components[i];
        }

      private:
        std::array<Field<D>, N> _components;
    };

    template <dim_t D>
    using ScalarField = VectorField<D, 1>;

    // function to compute the Divergence of a vector field at point X
    template <dim_t D>
    inline real divX(const VectorField<D, D> & function, const vector<D> & X)
    {
        real result = 0.0;
        for (int i = 0; i < D; ++i) {
            result += function[i].Df(i)(X);
        }
        return result;
    }

    // function to compute the gradient of a scalar field with respect to the reference
    // configuration at point X
    template <dim_t D>
    inline auto grad(const ScalarField<D> & function, const vector<D> & X)
    {
        vector<D> result;
        for (int i = 0; i < D; ++i) {
            result[i] = function[0].Df(i)(X);
        }
        return result;
    }

    // helper function to compute the gradient of a vector field with respect to the reference
    // configuration at point X (template with index sequence)
    template <dim_t D, std::size_t... I>
    inline VectorField<D, D> _grad(const ScalarField<D> & function, std::index_sequence<I...>)
    {
        return VectorField<D, D>({ function[0].Df(I)... });
    }

    // function to compute the gradient of a vector field with respect to the reference
    // configuration at point X
    template <dim_t D>
    inline VectorField<D, D> grad(const ScalarField<D> & function)
    {
        return _grad(function, std::make_index_sequence<D> {});
    }
}

#endif    //__MITO__FIELDS__

// end of file
