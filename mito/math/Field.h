// code guard
#if !defined(mito_math_Field_h)
#define mito_math_Field_h


namespace mito::math {

    // TODO: Add operator+ for scalar fields and reals

    // f(X,t) with (X \in R^D, t \in R) -> Y
    template <class X, class Y>
    class Field {

        // dimension of the X space
        static constexpr int D = size<X>::value;

      public:
        // constructors with function_t<X, Y>
        Field(const function_t<X, Y> & f) : _f(f), _Df() {}
        Field(function_t<X, Y> && f) : _f(f), _Df() {}
        Field(const function_t<X, Y> & f, const std::array<function_t<X, Y>, D> & Df) : _f(f), 
            _Df(Df) {}
        Field(const function_t<X, Y> & f, std::array<function_t<X, Y>, D> && Df) : _f(f), _Df(Df) {}
        Field(function_t<X, Y> && f, std::array<function_t<X, Y>, D> && Df) : _f(f), _Df(Df) {}

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

        inline auto operator()(const X & x) const
        {
            // evaluate _f
            return _f(x);
        }

        // TODO: Note that now this operator() is the same thing as the following but the only thing
        // that changes is the container structure
        inline auto operator()(const std::vector<X> & x) const
        {
            std::vector<Y> values(x.size());
            // evaluate operator() at all elements of X
            for (int i = 0; i < (int) x.size(); ++i) {
                values[i] = operator()(x[i]);
            }
            return values;
        }

        template <int Q>
        inline auto operator()(const fem::quadrature_field_t<Q, X> & x) const
        {
            fem::quadrature_field_t<Q, Y> values(x.n_elements());

            // evaluate operator() at all elements of x
            for (int e = 0; e < values.n_elements(); ++e) {
                for (int q = 0; q < Q; ++q) {
                    values[{ e, q }] = operator()(x[{ e, q }]);
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
        function_t<X, Y> _f;
        // the derivatives of f with respect to X (position in the reference configuration)
        std::array<function_t<X, Y>, D> _Df;
    };

    template <class X, class Y, std::size_t... I>
    inline auto _dSum(
        const field_t<X, Y> & fieldA, const field_t<X, Y> & fieldB, std::index_sequence<I...>)
    {
        // dimension of the X space
        static constexpr int D = size<X>::value;

        std::array<function_t<X, Y>, D> Df = { (fieldA.Df(I) + fieldB.Df(I))... };
        return Df;
    }

    template <class X, class Y>
    auto operator+(const field_t<X, Y> & fieldA, const field_t<X, Y> & fieldB)
    {
        // dimension of the X space
        static constexpr int D = size<X>::value;

        return field_t<X, Y>(
            fieldA.f() + fieldB.f(), _dSum(fieldA, fieldB, std::make_index_sequence<D> {}));
    }

    // helper function to compute the gradient of a vector field with respect to the reference
    // configuration (template with index sequence)
    template <int D, std::size_t... I>
    inline auto _grad(
        const scalar_field_t<D> & field, const vector_t<D> & x, std::index_sequence<I...>)
    {
        // all done
        return vector_t<D> { field.Df(I)(x)... };
    }

    // function to compute the gradient of a scalar field with respect to the reference
    // configuration at point x
    template <int D>
    inline auto grad(const scalar_field_t<D> & field, const vector_t<D> & x)
    {
        return _grad(field, x, std::make_index_sequence<D> {});
    }

    // helper function to compute the gradient of a vector field with respect to the reference
    // configuration (template with index sequence)
    template <int D, std::size_t... I>
    inline vector_field_t<D, D> _grad(const scalar_field_t<D> & field, std::index_sequence<I...>)
    {
        // TOFIX: capturing field by copy or by reference? Is it better to capture
        // {field.Df(I)...} intead? Is it possible to do so?
        return vector_field_t<D, D>(function_t<vector_t<D>, vector_t<D>>(
            [field](const vector_t<D> & x) { return vector_t<D> { field.Df(I)(x)... }; }));
    }

    // function to compute the gradient of a vector field with respect to the reference
    // configuration
    template <int D>
    inline vector_field_t<D, D> grad(const scalar_field_t<D> & field)
    {
        return _grad(field, std::make_index_sequence<D> {});
    }

    // function to compute the Divergence of a vector field at point X
    template <int D>
    inline real div(const vector_field_t<D, D> & field, const vector_t<D> & X)
    {
        real result = 0.0;
        for (int i = 0; i < D; ++i) {
            result += field.Df(i)(X)[i];
        }
        return result;
    }

    // helper function to compute the divergence of a vector field with respect to the reference
    // configuration at point X (template with index sequence)
    template <int D, std::size_t... I>
    inline scalar_field_t<D> _div(const vector_field_t<D, D> & field, std::index_sequence<I...>)
    {
        // TOFIX: capturing field by copy or by reference? Is it better to capture
        // {field.Df(I)...} intead? Is it possible to do so?
        return scalar_field_t<D>(function_t<vector_t<D>>(
            [field](const vector_t<D> & x) { return (field.Df(I)(x)[I] + ...); }));
    }

    // function to compute the divergence of a vector field with respect to the reference
    // configuration at point X
    template <int D>
    inline scalar_field_t<D> div(const vector_field_t<D, D> & field)
    {
        return _div(field, std::make_index_sequence<D> {});
    }
}


#endif    // mito_math_Field_h

// end of file
