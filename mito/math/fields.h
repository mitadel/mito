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
            std::vector<Y> values(x.size());
            // evaluate operator() at all elements of X
            for (int i = 0; i < (int) x.size(); ++i) {
                values[i] = operator()(x[i]);
            }
            return values;
        }

        template <int Q>
        inline auto operator()(const quadrature_field_t<Q, vector<D>> & x) const
        {
            quadrature_field_t<Q, Y> values(x.n_elements());

            // evaluate operator() at all elements of x
            for (int e = 0; e < values.n_elements(); ++e) {
                for (int q = 0; q < Q; ++q) {
                    values(e, q) = operator()(x(e, q));
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

    template <dim_t D, int N>
    using VectorField = Field<D, vector<N>>;

    template <dim_t D>
    using ScalarField = Field<D, real>;

    template <dim_t D, typename Y, std::size_t... I>
    inline auto _dSum(
        const Field<D, Y> & fieldA, const Field<D, Y> & fieldB, std::index_sequence<I...>)
    {
        std::array<Function<vector<D>, Y>, D> Df = { (fieldA.Df(I) + fieldB.Df(I))... };
        return Df;
    }

    template <dim_t D, typename Y>
    auto operator+(const Field<D, Y> & fieldA, const Field<D, Y> & fieldB)
    {
        return Field<D, Y>(
            fieldA.f() + fieldB.f(), _dSum(fieldA, fieldB, std::make_index_sequence<D> {}));
    }

    // function to compute the gradient of a scalar field with respect to the reference
    // configuration at point x
    template <dim_t D>
    inline auto grad(const ScalarField<D> & field, const vector<D> & x)
    {
        vector<D> result;
        for (dim_t i = 0; i < D; ++i) {
            result[i] = field.Df(i)(x);
        }
        return result;
    }

    // helper function to compute the gradient of a vector field with respect to the reference
    // configuration (template with index sequence)
    template <dim_t D, std::size_t... I>
    inline VectorField<D, D> _grad(const ScalarField<D> & field, std::index_sequence<I...>)
    {
        // TOFIX: capturing field by copy or by reference? Is it better to capture
        // {field.Df(I)...} intead? Is it possible to do so?
        return VectorField<D, D>(mito::Function<vector<D>, vector<D>>(
            [field](const vector<D> & x) { return vector<D> { field.Df(I)(x)... }; }));
    }

    // function to compute the gradient of a vector field with respect to the reference
    // configuration
    template <dim_t D>
    inline VectorField<D, D> grad(const ScalarField<D> & field)
    {
        return _grad(field, std::make_index_sequence<D> {});
    }

    // function to compute the Divergence of a vector field at point X
    template <dim_t D>
    inline real div(const VectorField<D, D> & field, const vector<D> & X)
    {
        real result = 0.0;
        for (dim_t i = 0; i < D; ++i) {
            result += field.Df(i)(X)[i];
        }
        return result;
    }

    // helper function to compute the divergence of a vector field with respect to the reference
    // configuration at point X (template with index sequence)
    template <dim_t D, std::size_t... I>
    inline ScalarField<D> _div(const VectorField<D, D> & field, std::index_sequence<I...>)
    {
        // TOFIX: capturing field by copy or by reference? Is it better to capture
        // {field.Df(I)...} intead? Is it possible to do so?
        return ScalarField<D>(mito::Function<vector<D>>(
            [field](const vector<D> & x) { return (field.Df(I)(x)[I] + ...); }));
    }

    // function to compute the divergence of a vector field with respect to the reference
    // configuration at point X
    template <dim_t D>
    inline ScalarField<D> div(const VectorField<D, D> & field)
    {
        return _div(field, std::make_index_sequence<D> {});
    }
}

#endif    //__MITO__FIELDS__

// end of file
