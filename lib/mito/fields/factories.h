// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fields {

    // factory for fields
    template <functions::function_c F>
    constexpr auto field(const F & f) -> field_t<F>
    {
        return field_t<F>(f);
    }

    // factory for fields
    template <functions::function_c F>
    constexpr auto field(F && f) -> field_t<F>
    {
        return field_t<F>(std::forward<F>(f));
    }

    // factory for fields (from functors)
    template <functions::functor_c F>
    constexpr auto field(F && f)
    {
        return field(mito::functions::function(std::forward<F>(f)));
    }

    // uniform field
    template <geometry::coordinates_c coordType, class Y>
    constexpr auto uniform_field(const Y & constant)
    {
        return field(mito::functions::constant<coordType>(constant));
    }
}


// end of file
