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

    // scalar + field
    template <field_c F>
    constexpr auto operator+(const tensor_or_scalar_c auto & a, const F & f)
    {
        return field<F::coordinate_type>(a + f.function());
    }

    // field + scalar
    constexpr auto operator+(const field_c auto & f, const tensor_or_scalar_c auto & a)
    {
        return a + f;
    }

    // scalar * fields
    template <field_c F>
    constexpr auto operator*(const tensor_or_scalar_c auto & a, const F & f)
    {
        return field<F::coordinate_type>(a * f.function());
    }

    // field * scalar
    constexpr auto operator*(const field_c auto & f, const tensor_or_scalar_c auto & a)
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

    // field / a
    constexpr auto operator/(const field_c auto & f, const scalar_t & a)
    {
        return (1.0 / a) * f;
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

    // a - field
    constexpr auto operator-(const tensor_or_scalar_c auto & a, const field_c auto & f)
    {
        return a + (-f);
    }

    // field - a
    constexpr auto operator-(const field_c auto & f, const tensor_or_scalar_c auto & a)
    {
        return f + (-a);
    }

    // a / field
    template <scalar_field_c F>
    constexpr auto operator/(const tensor_or_scalar_c auto & a, const F & f)
    {
        return field<F::coordinate_type>(a / f.function());
    }

    // field1 / field2
    template <field_c F1, scalar_field_c F2>
    constexpr auto operator/(const F1 & f1, const F2 & f2)
    requires(
        // {f1} and {f2} are defined on the same coordinates
        std::is_same_v<typename F1::coordinates_type, typename F2::coordinates_type>)
    {
        return field<F1::coordinate_type>(f1.function() / f2.function());
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
}


#endif

// end of file
