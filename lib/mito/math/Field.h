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
        // TOFIX: should these be passed by reference or by value?
        // constructors with function_t<X, Y>
        constexpr Field(const function_t<X, Y> & f) : _f(f), _Df() {}
        constexpr Field(const function_t<X, Y> & f, const std::array<function_t<X, Y>, D> & Df) :
            _f(f),
            _Df(Df)
        {}

        // default move constructor
        constexpr Field(Field &&) noexcept = default;

        // default move operator=
        constexpr Field & operator=(Field &&) noexcept = default;

        // default copy constructor
        constexpr Field(const Field &) = default;

        // default assignment operator
        constexpr Field & operator=(const Field &) = default;

        // delete default constructor
        constexpr Field() = delete;

        // destructor
        constexpr ~Field() {}

        constexpr auto operator()(const X & x) const -> Y
        {
            // evaluate _f
            return _f(x);
        }

      public:
        // accessor for function
        constexpr auto f() const noexcept -> const function_t<X, Y> & { return _f; }

        // accessor for function partial derivatives
        constexpr auto Df(int i) const -> const function_t<X, Y> &
        {
            // assert there exists the i-th partial derivative
            assert(i < (int) std::size(_Df));
            // return the i-th partial derivative
            return _Df[i];
        }

      private:
        // the function
        function_t<X, Y> _f;
        // the derivatives of f with respect to X (position in the reference configuration)
        std::array<function_t<X, Y>, D> _Df;
    };

    template <class X, class Y>
    constexpr auto operator+(const field_t<X, Y> & fieldA, const field_t<X, Y> & fieldB)
        -> field_t<X, Y>
    {
        // dimension of the X space
        constexpr int D = size<X>::value;

        // helper function to sum the derivatives of fieldA and fieldB
        constexpr auto _dSum = []<size_t... I>(
                                   const field_t<X, Y> & fieldA, const field_t<X, Y> & fieldB,
                                   std::index_sequence<I...>) {
            std::array<function_t<X, Y>, D> Df = { (fieldA.Df(I) + fieldB.Df(I))... };
            return Df;
        };

        return field_t<X, Y>(
            fieldA.f() + fieldB.f(), _dSum(fieldA, fieldB, std::make_index_sequence<D> {}));
    }

    // function to compute the gradient of a scalar field with respect to the reference
    // configuration at point x
    template <int D>
    constexpr auto grad(const scalar_field_t<D> & field, const vector_t<D> & x) -> vector_t<D>
    {
        // helper function to compute the gradient of a vector field with respect to the reference
        // configuration (template with index sequence)
        constexpr auto _grad = []<size_t... I>(
                                   const scalar_field_t<D> & field, const vector_t<D> & x,
                                   std::index_sequence<I...>) -> vector_t<D> {
            return vector_t<D> { field.Df(I)(x)... };
        };

        return _grad(field, x, std::make_index_sequence<D> {});
    }

    // function to compute the gradient of a vector field with respect to the reference
    // configuration
    template <int D>
    constexpr auto grad(const scalar_field_t<D> & field) -> vector_field_t<D, D>
    {
        // helper function to compute the gradient of a vector field with respect to the reference
        // configuration (template with index sequence)
        constexpr auto _grad = []<size_t... I>(
                                   const scalar_field_t<D> & field,
                                   std::index_sequence<I...>) -> vector_field_t<D, D> {
            return vector_field_t<D, D>(function_t<vector_t<D>, vector_t<D>>(
                [field](const vector_t<D> & x) { return vector_t<D> { field.Df(I)(x)... }; }));
        };

        return _grad(field, std::make_index_sequence<D> {});
    }

    // function to compute the Divergence of a vector field at point X
    template <int D>
    constexpr auto div(const vector_field_t<D, D> & field, const vector_t<D> & X) -> real
    {
        real result = 0.0;
        for (int i = 0; i < D; ++i) {
            result += field.Df(i)(X)[i];
        }
        return result;
    }

    // function to compute the divergence of a vector field with respect to the reference
    // configuration at point X
    template <int D>
    constexpr auto div(const vector_field_t<D, D> & field) -> scalar_field_t<D>
    {
        // helper function to compute the divergence of a vector field with respect to the reference
        // configuration at point X (template with index sequence)
        constexpr auto _div = []<size_t... I>(
                                  const vector_field_t<D, D> & field,
                                  std::index_sequence<I...>) -> scalar_field_t<D> {
            return scalar_field_t<D>(function_t<vector_t<D>>(
                [field](const vector_t<D> & x) { return (field.Df(I)(x)[I] + ...); }));
        };

        return _div(field, std::make_index_sequence<D> {});
    }
}


#endif    // mito_math_Field_h

// end of file
