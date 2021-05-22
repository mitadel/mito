#ifndef __MITO__FIELDS__
#define __MITO__FIELDS__

#include "function.h"
#include "../mito.h"
#include "../elements/quadrature_field.h"

namespace mito {

    // TODO: Add operator+ for scalar fields and reals

    // f(X,t) with (X \in R^D, t \in R) -> R
    template <dim_t D, typename Y = real>
    class Field {

        // typedef for a scalar valued function
        using function_t = Function<vector<D>, Y>;
        using functor_t = functor<vector<D>, Y>;

      public:
        // constructors with function_t
        Field(const function_t & f) : _f(f), _Df() {}
        Field(function_t && f) : _f(f), _Df() {}
        Field(const function_t & f, const std::array<function_t, D> & Df) : _f(f), _Df(Df) {}
        Field(const function_t & f, std::array<function_t, D> && Df) : _f(f), _Df(Df) {}
        Field(function_t && f, std::array<function_t, D> && Df) : _f(f), _Df(Df) {}

        // constructors with functor_t
        Field(const functor_t & f) : _f(f), _Df() {}
        Field(functor_t && f) : _f(f), _Df() {}
        Field(const functor_t & f, const std::array<functor_t, D> & Df) : _f(f), _Df(Df) {}
        Field(const functor_t & f, std::array<functor_t, D> && Df) : _f(f), _Df(Df) {}
        Field(functor_t && f, std::array<functor_t, D> && Df) : _f(f), _Df(Df) {}

        // default move constructor
        Field(Field &&) = default;

        // default move operator=
        Field & operator=(Field &&) = default;

        // default copy constructor
        Field(const Field &) = default;

        // default assignment operator
        Field & operator=(const Field &) = default;

        // delete default constructor
        Field() = delete;

        // destructor
        ~Field() {}

        inline auto operator()(const vector<D> & x) const
        {
            // evaluate _f
            return _f(x);
        }

        inline auto operator()(const std::vector<vector<D>> & x) const
        {
            std::vector<real> values(x.size(), 0.0);
            // evaluate operator() at all elements of X
            for (int i = 0; i < (int) x.size(); ++i) {
                values[i] = operator()(x[i]);
            }
            return values;
        }

        template <int Q>
        inline auto operator()(const quadrature_field_t<Q, D> & x) const
        {
            quadrature_field_t<Q, D> values(x.n_elements());

            // evaluate operator() at all elements of x
            for (int e = 0; e < values.n_elements(); ++e) {
                for (int q = 0; q < Q; ++q) {
                    values(e, q)[0] = operator()(x(e, q));
                }
            }

            // all done
            return values;
        }

      public:
        // accessor for function
        inline const auto & f() const { return _f; }

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

    template <dim_t D, std::size_t... I>
    inline auto _dSum(const Field<D> & fieldA, const Field<D> & fieldB, std::index_sequence<I...>)
    {
        std::array<Function<vector<D>>, D> Df = { (fieldA.Df(I) + fieldB.Df(I))... };
        return Df;
    }

    template <int D>
    Field<D> operator+(const Field<D> & fieldA, const Field<D> & fieldB)
    {
        return Field<D>(
            fieldA.f() + fieldB.f(), _dSum(fieldA, fieldB, std::make_index_sequence<D> {}));
    }

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
            assert(i < (int) _components.size());
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

    // helper function to compute the divergence of a vector field with respect to the reference
    // configuration at point X (template with index sequence)
    template <dim_t D, std::size_t... I>
    inline ScalarField<D> _div(const VectorField<D, D> & function, std::index_sequence<I...>)
    {
        return ScalarField<D>({ template_sum(function[I].Df(I)...) });
    }

    // function to compute the divergence of a vector field with respect to the reference
    // configuration at point X
    template <dim_t D>
    inline ScalarField<D> div(const VectorField<D, D> & function)
    {
        return _div(function, std::make_index_sequence<D> {});
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
