// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_fields_arithmetic_h)
#define mito_manifolds_fields_arithmetic_h


// Arithmetic operations on Fields
namespace mito::manifolds {

    // addition of fields fa + fb
    template <field_c F1, field_c F2>
    constexpr auto operator+(const F1 & fA, const F2 & fB)
    requires(compatible_fields_c<F1, F2>)
    {
        return field(fA.function() + fB.function());
    }

    // scalar + field
    template <field_c F>
    constexpr auto operator+(const tensor_or_scalar_c auto & a, const F & f)
    {
        return field(a + f.function());
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
        return field(a * f.function());
    }

    // field * scalar
    constexpr auto operator*(const field_c auto & f, const tensor_or_scalar_c auto & a)
    {
        return a * f;
    }

    // product of fields
    template <field_c F1, field_c F2>
    constexpr auto operator*(const F1 & fA, const F2 & fB)
    requires(compatible_fields_c<F1, F2>)
    {
        return field(fA.function() * fB.function());
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
        return field(a / f.function());
    }

    // field1 / field2
    template <field_c F1, scalar_field_c F2>
    constexpr auto operator/(const F1 & f1, const F2 & f2)
    requires(compatible_fields_c<F1, F2>)
    {
        return field(f1.function() / f2.function());
    }
}


#endif

// end of file
