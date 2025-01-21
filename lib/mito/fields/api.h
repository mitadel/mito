// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fields {

    // field alias
    template <functions::function_c F>
    using field_t = Field<F>;

    // factory for fields
    template <functions::function_c F>
    constexpr auto field(const F & f) -> field_t<F>;

    // factory for fields
    template <functions::function_c F>
    constexpr auto field(F && f) -> field_t<F>;

    // factory for fields (from functors)
    template <functions::functor_c F>
    constexpr auto field(F && f) -> field_t<F>;

    // uniform field
    template <geometry::coordinates_c coordType, class Y>
    constexpr auto uniform_field(const Y & constant);

    // the order N identity tensor in D dimensions
    template <geometry::coordinates_c coordType, int N>
    constexpr auto identity_tensor_field = uniform_field<coordType>(tensor::identity<N>);
}


// end of file
