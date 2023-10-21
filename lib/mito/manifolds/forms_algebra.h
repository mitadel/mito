// code guard
#if !defined(mito_manifold_forms_h)
#define mito_manifold_forms_h


namespace mito::manifolds {

    // addition of forms fa + fb
    template <class F1, class F2>
    constexpr auto operator+(const one_form_t<F1> & fA, const one_form_t<F2> & fB)
    requires(one_form_t<F1>::dim == one_form_t<F2>::dim)
    {
        // the dimension of the vector space
        constexpr int D = one_form_t<F1>::dim;
        return one_form([fA, fB](const mito::vector_t<D> & x) { return fA(x) + fB(x); });
    }

    // scalar * form
    template <class F>
    constexpr auto operator*(const real & a, const one_form_t<F> & f)
    {
        // the dimension of the vector space
        constexpr int D = one_form_t<F>::dim;
        return one_form([a, f](const mito::vector_t<D> & x) { return a * f(x); });
    }

    // form * scalar
    template <class F>
    constexpr auto operator*(const one_form_t<F> & f, const real & a)
    {
        return a * f;
    }

    // unary operator- for forms
    template <class F>
    constexpr auto operator-(const one_form_t<F> & f)
    {
        return -1.0 * f;
    }

    // subtraction of forms fa - fb
    template <class F1, class F2>
    constexpr auto operator-(const one_form_t<F1> & fA, const one_form_t<F2> & fB)
    {
        return fA + (-fB);
    }

    // the wedge product of two one-forms
    template <class F1, class F2>
    constexpr auto wedge(const one_form_t<F1> & a_tilda, const one_form_t<F2> & b_tilda)
    requires(
        // {a_tilda} and {b_tilda} are defined on the same vector space
        one_form_t<F1>::dim == one_form_t<F2>::dim)
    {
        // return a {tensor} that, when contracted with {x} and {y}...
        return mito::manifolds::tensor(
            [a_tilda, b_tilda]<class X, class Y>(const X & x, const Y & y) -> auto {
                // ... returns the {scalar} prescribed by the wedge product
                return a_tilda(x) * b_tilda(y) - b_tilda(x) * a_tilda(y);
            });
    }

    // the wedge product of three one-forms
    template <class F1, class F2, class F3>
    constexpr auto wedge(
        const one_form_t<F1> & a_tilda, const one_form_t<F2> & b_tilda,
        const one_form_t<F3> & c_tilda)
    requires(
        // {a_tilda}, {b_tilda} and {c_tilda} are defined on the same vector space
        one_form_t<F1>::dim == one_form_t<F2>::dim && one_form_t<F1>::dim == one_form_t<F3>::dim)
    {
        // return a {tensor} that, when contracted with {x}, {y} and {z}...
        return mito::manifolds::tensor(
            [a_tilda, b_tilda, c_tilda]<class X, class Y, class Z>(
                const X & x, const Y & y, const Z & z) -> auto {
                // ... returns the {scalar} prescribed by the wedge product
                return a_tilda(x) * wedge(b_tilda, c_tilda)(y, z)
                     + b_tilda(x) * wedge(c_tilda, a_tilda)(y, z)
                     + c_tilda(x) * wedge(a_tilda, b_tilda)(y, z);
            });
    }

    // the tensor product of two one-forms
    template <class F1, class F2>
    constexpr auto tensor(const one_form_t<F1> & a_tilda, const one_form_t<F2> & b_tilda)
    requires(
        // {a_tilda} and {b_tilda} are defined on the same vector space
        one_form_t<F1>::dim == one_form_t<F2>::dim)
    {
        // return a {tensor} that, when contracted with {x} and {y}...
        return mito::manifolds::tensor(
            [a_tilda, b_tilda]<class X, class Y>(const X & x, const Y & y) -> auto {
                // ... returns the {scalar} prescribed by the tensor product
                return a_tilda(x) * b_tilda(y);
            });
    }

    // the tensor product of three one-forms
    template <class F1, class F2, class F3>
    constexpr auto tensor(
        const one_form_t<F1> & a_tilda, const one_form_t<F2> & b_tilda,
        const one_form_t<F3> & c_tilda)
    requires(
        // {a_tilda}, {b_tilda} and {c_tilda} are defined on the same vector space
        one_form_t<F1>::dim == one_form_t<F2>::dim && one_form_t<F1>::dim == one_form_t<F3>::dim)
    {
        // return a {tensor} that, when contracted with {x}, {y} and {z}...
        return mito::manifolds::tensor(
            [a_tilda, b_tilda, c_tilda]<class X, class Y, class Z>(
                const X & x, const Y & y, const Z & z) -> auto {
                // ... returns the {scalar} prescribed by the tensor product
                return a_tilda(x) * b_tilda(y) * c_tilda(z);
            });
    }
}


#endif

// end of file
