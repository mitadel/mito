
// code guard
#if !defined(mito_math_Function_h)
#define mito_math_Function_h


namespace mito::math {

    // We need a class function to explicitly put the return value Y in the template
    template <class X, class Y>
    class Function {

      public:
        // TOFIX: should these be passed by reference or by value?
        // constructor
        constexpr Function(const functor_t<X, Y> & f) : _functor(f) {}
        // constructor for lambdas
        template <class F>
        constexpr Function(F f) : _functor(f)
        {}
        // default constructor
        constexpr Function() = default;
        // copy constructor
        constexpr Function(const Function &) = default;
        // move constructor
        constexpr Function(Function &&) noexcept = default;
        // destructor
        constexpr ~Function() {};
        // assignment operator
        constexpr Function & operator=(const Function &) = default;
        // move operator=
        constexpr Function & operator=(Function &&) noexcept = default;

        constexpr auto operator()(const X & x) const
        {
            // evaluate _f
            return _functor(x);
        }

        constexpr auto operator[](int i) const
        {
            return function_t<X, scalar_t>([this, i](const X & x) { return this->_functor(x)[i]; });
        }

      private:
        // helper function to vectorize an array of scalar-valued functions
        template <size_t N, size_t... I>
        constexpr auto _vectorize(
            const function_t<X, scalar_t> (&f_list)[N], std::index_sequence<I...>)
        {
            return function_t<X, vector_t<N>>(
                [f_list](const X & x) { return vector_t<N> { f_list[I](x)... }; });
        }

      public:
        // constructor for a vector-valued function from an array of scalar-valued functions
        template <size_t N>
        constexpr Function(const function_t<X, scalar_t> (&f_list)[N])
        requires(Y::size == N)
            : _functor(_vectorize(f_list, std::make_index_sequence<N> {}))
        {}

        // cast operator from Function<X, Y> to functor_t<X, Y>
        constexpr operator functor_t<X, Y>() const { return _functor; }


      private:
        const functor_t<X, Y> _functor;
    };
}

#endif    // mito_math_Function_h

// end of file
