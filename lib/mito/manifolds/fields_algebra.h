// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_fields_algebra_h)
#define mito_manifolds_fields_algebra_h


// Algebraic operations on Fields
namespace mito::manifolds {

    // addition of fields fa + fb
    template <field_c F1, field_c F2>
    constexpr auto operator+(const F1 & fA, const F2 & fB)
    requires(
        // {fA} and {fB} are defined on the same coordinates
        std::is_same_v<typename F1::coordinates_type, typename F2::coordinates_type>)
    {
        return field<F1::coordinate_type>(fA.function() + fB.function());
    }

    // scalar * fields
    template <field_c F>
    constexpr auto operator*(const real & a, const F & f)
    {
        return field<F::coordinate_type>(a * f.function());
    }

    // field * scalar
    constexpr auto operator*(const field_c auto & f, const real & a)
    {
        return a * f;
    }

    // product of fields
    template <field_c F1, field_c F2>
    constexpr auto operator*(const F1 & fA, const F2 & fB)
    requires(
        // {fA} and {fB} are defined on the same coordinates
        std::is_same_v<typename F1::coordinates_type, typename F2::coordinates_type>)
    {
        return field<F1::coordinate_type>(fA.function() * fB.function());
    }

    // unary operator- for fields
    constexpr auto operator-(const field_c auto & f)
    {
        return -1.0 * f;
    }

    // subtraction of fields fa - fb
    constexpr auto operator-(const field_c auto & fA, const field_c auto & fB)
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
    constexpr auto wedge(const one_form_field_c auto & fA)
    {
        // return a {fA}
        return fA;
    }

    // the wedge product of two fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2>
    constexpr auto wedge(const F1 & fA, const F1 & fB)
    requires(
        // {fA} and {fB} are defined on the same coordinates
        std::is_same_v<typename F1::coordinates_type, typename F2::coordinates_type>)
    {
        using coordinates_type = typename F1::coordinates_type;
        return field(functions::function(
            [fA, fB](const coordinates_type & x) -> auto { return wedge(fA(x), fB(x)); }));
    }

    // the wedge product of three fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2, one_form_field_c F3>
    constexpr auto wedge(const F1 & fA, const F2 & fB, const F3 & fC)
    requires(
        // {fA}, {fB} and {fC} are defined on the same coordinates
        std::is_same_v<typename F1::coordinates_type, typename F2::coordinates_type>
        && std::is_same_v<typename F2::coordinates_type, typename F3::coordinates_type>)
    {
        using coordinates_type = typename F1::coordinates_type;
        return field(functions::function([fA, fB, fC](const coordinates_type & x) -> auto {
            return wedge(fA(x), fB(x), fC(x));
        }));
    }

    // the tensor product of two fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2>
    constexpr auto tensor(const F1 & fA, const F2 & fB)
    requires(
        // {fA} and {fB} are defined on the same coordinates
        std::is_same_v<typename F1::coordinates_type, typename F2::coordinates_type>)
    {
        using coordinates_type = typename F1::coordinates_type;
        return field(functions::function(
            [fA, fB](const coordinates_type & x) -> auto { return tensor(fA(x), fB(x)); }));
    }

    // the tensor product of three fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2, one_form_field_c F3>
    constexpr auto tensor(const F1 & fA, const F2 & fB, const F3 & fC)
    requires(
        // {fA}, {fB} and {fC} are defined on the same coordinates
        std::is_same_v<typename F1::coordinates_type, typename F2::coordinates_type>
        && std::is_same_v<typename F2::coordinates_type, typename F3::coordinates_type>)
    {
        using coordinates_type = typename F1::coordinates_type;
        return field(functions::function([fA, fB, fC](const coordinates_type & x) -> auto {
            return tensor(fA(x), fB(x), fC(x));
        }));
    }
}


#endif

// end of file
