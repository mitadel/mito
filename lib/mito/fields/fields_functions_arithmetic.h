// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// Arithmetic hybrid operations on Fields and Functions
namespace mito::fields {

    // addition of fields fa + fb
    template <field_c F1, functions::function_c F2>
    constexpr auto operator+(const F1 & fA, const F2 & fB)
    {
        return field(fA.function() + fB);
    }

    // addition of fields fa + fb
    template <functions::function_c F1, field_c F2>
    constexpr auto operator+(const F1 & fA, const F2 & fB)
    {
        return fB + fA;
    }

    // product of fields
    template <field_c F1, functions::function_c F2>
    constexpr auto operator*(const F1 & fA, const F2 & fB)
    {
        return field(fA.function() * fB);
    }

    // product of fields
    template <functions::function_c F1, field_c F2>
    constexpr auto operator*(const F1 & fA, const F2 & fB)
    {
        return field(fA * fB.function());
    }

    // subtraction of fields fa - fb
    constexpr auto operator-(const field_c auto & fA, const functions::function_c auto & fB)
    {
        return fA + (-fB);
    }

    // subtraction of fields fa - fb
    constexpr auto operator-(const functions::function_c auto & fA, const field_c auto & fB)
    {
        return fA + (-fB);
    }

    // field1 / field2
    template <field_c F1, functions::scalar_function_c F2>
    constexpr auto operator/(const F1 & f1, const F2 & f2)
    {
        return field(f1.function() / f2);
    }

    // field1 / field2
    template <functions::function_c F1, scalar_field_c F2>
    constexpr auto operator/(const F1 & f1, const F2 & f2)
    {
        return field(f1 / f2.function());
    }
}


// end of file
