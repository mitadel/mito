// code guard
#if !defined(mito_math_fields_algebra_h)
#define mito_math_fields_algebra_h


// Algebraic operations on Fields
namespace mito::manifolds {

    // addition of fields fa + fb
    template <class F1, class F2>
    constexpr auto operator+(const field_t<F1> & fA, const field_t<F2> & fB)
    requires(field_t<F1>::dim == field_t<F2>::dim)
    {
        // the dimension of the vector space
        constexpr int D = field_t<F1>::dim;
        return field([fA, fB](const mito::vector_t<D> & x) { return fA(x) + fB(x); });
    }

    // scalar * fields
    template <class F>
    constexpr auto operator*(const real & a, const field_t<F> & f)
    {
        // the dimension of the vector space
        constexpr int D = field_t<F>::dim;
        return field([a, f](const mito::vector_t<D> & x) { return a * f(x); });
    }

    // field * scalar
    template <class F>
    constexpr auto operator*(const field_t<F> & f, const real & a)
    {
        return a * f;
    }

    // TOFIX: this method is dangerous as it might misleadingly lead to think that a dot product is
    //          being performed. Let's activate it only if we see we need it.
    // // tensor product of fields
    // template <class F1, class F2>
    // constexpr auto operator*(const field_t<F1> & fA, const field_t<F2> & fB)
    // {
    //     // the dimension of the vector space
    //     constexpr int D = field_t<F1>::dim;
    //     // NOTE: why fA(x) * fB and not fA * fB(x)? be aware that we are introducing a convention
    //     // here
    //     return field([fA, fB](const mito::vector_t<D> & x) { return fA(x) * fB; });
    // }

    // unary operator- for fields
    template <class F>
    constexpr auto operator-(const field_t<F> & f)
    {
        return -1.0 * f;
    }

    // subtraction of fields fa - fb
    template <class F1, class F2>
    constexpr auto operator-(const field_t<F1> & fA, const field_t<F2> & fB)
    {
        return fA + (-fB);
    }

    // f^(-1)
    template <class F>
    constexpr auto inverse(const field_t<F> & f)
    {
        constexpr int D = field_t<F>::dim;
        return function([f](const mito::vector_t<D> & x) { return inverse(f(x)); });
    }

    // det(f)
    template <class F>
    constexpr auto determinant(const field_t<F> & f)
    {
        constexpr int D = field_t<F>::dim;
        return field([f](const mito::vector_t<D> & x) { return determinant(f(x)); });
    }

    // sqrt(f)
    template <class F>
    constexpr auto sqrt(const field_t<F> & f)
    {
        constexpr int D = field_t<F>::dim;
        return field([f](const mito::vector_t<D> & x) { return std::sqrt(f(x)); });
    }
}


#endif

// end of file
