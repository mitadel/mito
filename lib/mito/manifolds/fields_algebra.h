// code guard
#if !defined(mito_math_fields_algebra_h)
#define mito_math_fields_algebra_h


// Algebraic operations on Fields
namespace mito::manifolds {

    // addition of fields fa + fb
    template <class F1, class F2>
    constexpr auto operator+(const field_t<F1> & fA, const field_t<F2> & fB)
    requires(
        // {fA} and {fB} are defined on the same coordinates
        std::is_same_v<
            typename field_t<F1>::coordinates_type, typename field_t<F2>::coordinates_type>)
    {
        using coordinates_type = typename field_t<F1>::coordinates_type;
        return field([fA, fB](const coordinates_type & x) { return fA(x) + fB(x); });
    }

    // scalar * fields
    template <class F>
    constexpr auto operator*(const real & a, const field_t<F> & f)
    {
        using coordinates_type = typename field_t<F>::coordinates_type;
        return field([a, f](const coordinates_type & x) { return a * f(x); });
    }

    // field * scalar
    template <class F>
    constexpr auto operator*(const field_t<F> & f, const real & a)
    {
        return a * f;
    }

    // product of fields
    template <class F1, class F2>
    constexpr auto operator*(const field_t<F1> & fA, const field_t<F2> & fB)
    requires(
        // {fA} and {fB} are defined on the same coordinates
        std::is_same_v<
            typename field_t<F1>::coordinates_type, typename field_t<F2>::coordinates_type>)
    {
        using coordinates_type = typename field_t<F1>::coordinates_type;
        return field([fA, fB](const coordinates_type & x) { return fA(x) * fB(x); });
    }

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
        using coordinates_type = typename field_t<F>::coordinates_type;
        return function([f](const coordinates_type & x) { return inverse(f(x)); });
    }

    // det(f)
    template <class F>
    constexpr auto determinant(const field_t<F> & f)
    {
        using coordinates_type = typename field_t<F>::coordinates_type;
        return field(
            [f](const coordinates_type & x) -> mito::scalar_t { return determinant(f(x)); });
    }

    // sqrt(f)
    template <class F>
    constexpr auto sqrt(const field_t<F> & f)
    {
        using coordinates_type = typename field_t<F>::coordinates_type;
        return field([f](const coordinates_type & x) -> mito::scalar_t { return std::sqrt(f(x)); });
    }

    // the wedge product of one field of one-forms (trivial case)
    template <class F1>
    constexpr auto wedge(const field_t<F1> & fA)
    {
        // return a {fA}
        return fA;
    }

    // the wedge product of two fields of one-forms
    template <class F1, class F2>
    constexpr auto wedge(const field_t<F1> & fA, const field_t<F2> & fB)
    requires(
        // {fA} and {fB} are defined on the same coordinates
        std::is_same_v<
            typename field_t<F1>::coordinates_type, typename field_t<F2>::coordinates_type>)
    {
        using coordinates_type = typename field_t<F1>::coordinates_type;
        return field([fA, fB](const coordinates_type & x) -> auto { return wedge(fA(x), fB(x)); });
    }

    // the wedge product of three fields of one-forms
    template <class F1, class F2, class F3>
    constexpr auto wedge(const field_t<F1> & fA, const field_t<F2> & fB, const field_t<F3> & fC)
    requires(
        // {fA}, {fB} and {fC} are defined on the same coordinates
        std::is_same_v<
            typename field_t<F1>::coordinates_type, typename field_t<F2>::coordinates_type>
        && std::is_same_v<
            typename field_t<F2>::coordinates_type, typename field_t<F3>::coordinates_type>)
    {
        using coordinates_type = typename field_t<F1>::coordinates_type;
        return field([fA, fB, fC](const coordinates_type & x) -> auto {
            return wedge(fA(x), fB(x), fC(x));
        });
    }

    // the tensor product of two fields of one-forms
    template <class F1, class F2>
    constexpr auto tensor(const field_t<F1> & fA, const field_t<F2> & fB)
    requires(
        // {fA} and {fB} are defined on the same coordinates
        std::is_same_v<
            typename field_t<F1>::coordinates_type, typename field_t<F2>::coordinates_type>)
    {
        using coordinates_type = typename field_t<F1>::coordinates_type;
        return field([fA, fB](const coordinates_type & x) -> auto { return tensor(fA(x), fB(x)); });
    }

    // the tensor product of three fields of one-forms
    template <class F1, class F2, class F3>
    constexpr auto tensor(const field_t<F1> & fA, const field_t<F2> & fB, const field_t<F3> & fC)
    requires(
        // {fA}, {fB} and {fC} are defined on the same coordinates
        std::is_same_v<
            typename field_t<F1>::coordinates_type, typename field_t<F2>::coordinates_type>
        && std::is_same_v<
            typename field_t<F2>::coordinates_type, typename field_t<F3>::coordinates_type>)
    {
        using coordinates_type = typename field_t<F1>::coordinates_type;
        return field([fA, fB, fC](const coordinates_type & x) -> auto {
            return tensor(fA(x), fB(x), fC(x));
        });
    }
}


#endif

// end of file
