// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_fields_algebra_h)
#define mito_manifolds_fields_algebra_h


// Algebraic operations on Fields
namespace mito::manifolds {

    // f^(-1)
    template <tensor_field_c F>
    constexpr auto inverse(const F & f)
    {
        using coordinates_type = typename F::coordinates_type;
        return field(
            functions::function([f](const coordinates_type & x) { return inverse(f(x)); }));
    }

    // det(f)
    template <tensor_field_c F>
    constexpr auto determinant(const F & f)
    {
        using coordinates_type = typename F::coordinates_type;
        return field(functions::function(
            [f](const coordinates_type & x) -> mito::scalar_t { return determinant(f(x)); }));
    }

    // sqrt(f)
    template <scalar_field_c F>
    constexpr auto sqrt(const F & f)
    {
        using coordinates_type = typename F::coordinates_type;
        return field(functions::function(
            [f](const coordinates_type & x) -> mito::scalar_t { return std::sqrt(f(x)); }));
    }
}


#endif

// end of file
