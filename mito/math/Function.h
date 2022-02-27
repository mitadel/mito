
// code guard
#if !defined(mito_math_Function_h)
#define mito_math_Function_h


namespace mito::math {

    // We need a class function to explicitly put the return value Y in the template
    template <class X, class Y>
    class Function {

      public:
        using input_type = X;
        using output_type = Y; 

      public:
        // constructor
        inline Function(const functor_t<X, Y> & f) : _functor(f) {}
        // constructor for lambdas
        template <class F> 
        inline Function(F f) : _functor(f) {}
        // default constructor
        inline Function() = default;
        // copy constructor
        inline Function(const Function &) = default;
        // move constructor
        inline Function(Function &&) = default;
        // destructor
        inline ~Function() {};
        // assignment operator
        inline Function & operator=(const Function &) = default;
        // move operator=
        inline Function & operator=(Function &&) = default;

        inline auto operator()(const X & x) const
        {
            // evaluate _f
            return _functor(x);
        }

        inline auto operator[](int i) const
        {
            return function_t<X, scalar_t>([this, i](const X & x) { return this->_functor(x)[i]; });
        }

      private:
        // helper function
        template <class... scalar_function>
        constexpr auto _vectorize(scalar_function... f_list)
        {
            constexpr int N = Y::size;
            return function_t<X, vector_t<N>>(
                [f_list...](const X & x) { return vector_t<N> { f_list(x)... }; });
        };

      public:
        template <class... scalar_function>
        inline Function(scalar_function... f_list) requires(
            // all elements in the variadic template are scalar-valued functions
            (std::is_same_v<typename scalar_function::output_type, mito::real> &&...)
            // the number of input arguments matches the dimension of the output
            && sizeof...(scalar_function) == Y::size) :
            // create a vector-valued function from a list of scalar-valued functions
            _functor(_vectorize(f_list...)) {}

        // cast operator from Function<X, Y> to functor_t<X, Y>
        inline operator functor_t<X, Y>() const { return _functor; }

      private:
        const functor_t<X, Y> _functor;
    };

    // Algebraic operations on Function
    // TODO: see if expression templates can be used to avoid copies with intermediate results

    // fa + fb
    template <class X, class Y>
    function_t<X, Y> operator+(const function_t<X, Y> & fA, const function_t<X, Y> & fB)
    {
        return function_t<X, Y>([fA, fB](const X & x) { return fA(x) + fB(x); });
    }

    // fa * fb
    template <class X, class Y>
    function_t<X, Y> operator*(const function_t<X, Y> & fA, const function_t<X, Y> & fB)
    {
        return function_t<X, Y>([fA, fB](const X & x) { return fA(x) * fB(x); });
    }

    // y * f (inner product)
    template <class X, class Y>
    function_t<X, typename type<Y>::value> operator*(const Y & y, const function_t<X, Y> & f) requires(
        Y::size != 1)
    {
        return function_t<X, typename type<Y>::value>([y, f](const X & x) { return y * f(x); });
    }

    // f * y (inner product)
    template <class X, class Y>
    function_t<X, typename type<Y>::value> operator*(const function_t<X, Y> & f, const Y & y) requires(
        Y::size != 1)
    {
        return y * f;
    }

    // a * f
    template <class X, class Y>
    function_t<X, Y> operator*(const real & a, const function_t<X, Y> & f)
    {
        return function_t<X, Y>([a, f](const X & x) { return a * f(x); });
    }

    // f * a
    template <class X, class Y>
    function_t<X, Y> operator*(const function_t<X, Y> & f, const real & a)
    {
        return a * f;
    }

    // f / a
    template <class X, class Y>
    function_t<X, Y> operator/(const function_t<X, Y> & f, const real & a)
    {
        return (1.0 / a) * f;
    }

    // -f
    template <class X, class Y>
    function_t<X, Y> operator-(const function_t<X, Y> & f)
    {
        return -1.0 * f;
    }

    // fa - fb
    template <class X, class Y>
    function_t<X, Y> operator-(const function_t<X, Y> & fA, const function_t<X, Y> & fB)
    {
        return fA + (-fB);
    }

    // Special algebraic functions for scalar functions
    // a / f
    template <class X>
    function_t<X, scalar_t> operator/(const real & a, const function_t<X, scalar_t> & f)
    {
        return function_t<X, scalar_t>([a, f](const X & x) { return a / f(x); });
    }

    // f1 / f2
    template <class X, class Y>
    function_t<X, Y> operator/(const function_t<X, Y> & f1, const function_t<X, scalar_t> & f2)
    {
        return function_t<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // a + f
    template <class X>
    function_t<X, scalar_t> operator+(const real & a, const function_t<X, scalar_t> & f)
    {
        return function_t<X, scalar_t>([a, f](const X & x) { return a + f(x); });
    }

    // f + a
    template <class X>
    function_t<X, scalar_t> operator+(const function_t<X, scalar_t> & f, const real & a)
    {
        return a + f;
    }

    // a - f
    template <class X>
    function_t<X, scalar_t> operator-(const real & a, const function_t<X, scalar_t> & f)
    {
        return a + (-f);
    }

    // f - a
    template <class X>
    function_t<X, scalar_t> operator-(const function_t<X, scalar_t> & f, const real & a)
    {
        return f + (-a);
    }

    // Binary operators for functor_t<X, Y> and function_t<X, Y>, such as:
    // fa + fb
    template <class X, class Y>
    function_t<X, Y> operator+(const function_t<X, Y> & fA, const functor_t<X, Y> & fB)
    {
        return fA + function_t<X, Y>(fB);
    }

    // fa + fb
    template <class X, class Y>
    function_t<X, Y> operator+(const functor_t<X, Y> & fA, const function_t<X, Y> & fB)
    {
        return fB + fA;
    }

    // fa * fb
    template <class X, class Y>
    function_t<X, Y> operator*(const function_t<X, Y> & fA, const functor_t<X, Y> & fB)
    {
        return fA * function_t<X, Y>(fB);
    }

    // fa * fb
    template <class X, class Y>
    function_t<X, Y> operator*(const functor_t<X, Y> & fA, const function_t<X, Y> & fB)
    {
        return fB * fA;
    }

    // fa - fb
    template <class X, class Y>
    function_t<X, Y> operator-(const function_t<X, Y> & fA, const functor_t<X, Y> & fB)
    {
        return fA - function_t<X, Y>(fB);
    }

    // fa - fb
    template <class X, class Y>
    function_t<X, Y> operator-(const functor_t<X, Y> & fA, const function_t<X, Y> & fB)
    {
        return -(fB - fA);
    }

    // f1 / f2
    template <class X, class Y>
    function_t<X, Y> operator/(const function_t<X, Y> & f1, const functor_t<X, scalar_t> & f2)
    {
        return function_t<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // f1 / f2
    template <class X, class Y>
    function_t<X, Y> operator/(const functor_t<X, Y> & f1, const function_t<X, scalar_t> & f2)
    {
        return function_t<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // Binary operators for Y(const X &) and function_t<X, Y>, such as:
    // fa + fb
    template <class X, class Y>
    function_t<X, Y> operator+(const function_t<X, Y> & fA, Y fB(const X &))
    {
        return fA + function_t<X, Y>(fB);
    }

    // fa + fb
    template <class X, class Y>
    function_t<X, Y> operator+(Y fA(const X &), const function_t<X, Y> & fB)
    {
        return fB + fA;
    }

    // fa * fb
    template <class X, class Y>
    function_t<X, Y> operator*(const function_t<X, Y> & fA, Y fB(const X &))
    {
        return fA * function_t<X, Y>(fB);
    }

    // fa * fb
    template <class X, class Y>
    function_t<X, Y> operator*(Y fA(const X &), const function_t<X, Y> & fB)
    {
        return fB * fA;
    }

    // fa - fb
    template <class X, class Y>
    function_t<X, Y> operator-(const function_t<X, Y> & fA, Y fB(const X &))
    {
        return fA - function_t<X, Y>(fB);
    }

    // fa - fb
    template <class X, class Y>
    function_t<X, Y> operator-(Y fA(const X &), const function_t<X, Y> & fB)
    {
        return -(fB - fA);
    }

    // f1 / f2
    template <class X, class Y>
    function_t<X, Y> operator/(const function_t<X, Y> & f1, real f2(const X &))
    {
        return function_t<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // f1 / f2
    template <class X, class Y>
    function_t<X, Y> operator/(Y f1(const X &), const function_t<X, scalar_t> & f2)
    {
        return function_t<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }
}

#endif    // mito_math_Function_h

// end of file
