// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_fields_api_h)
#define mito_fields_api_h


namespace mito::fields {

    // field alias
    template <functions::function_c F>
    using field_t = Field<F>;

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
    constexpr auto identity_tensor_field = uniform_field<coordType>(mito::identity<N>);

    // the function associating to a set of coordinates of type {coordType} its N-th coordinate
    template <geometry::coordinates_c coordType, int N>
    [[maybe_unused]] constexpr auto coordinate = mito::functions::component<coordType, N>();

    // construct a one-form based on its metric-equivalent vector field
    template <vector_field_c vectorFieldT, symmetric_tensor_field_c tensorFieldT>
    constexpr auto one_form(const vectorFieldT & vector, const tensorFieldT & metric)
    requires(compatible_fields_c<vectorFieldT, tensorFieldT>);

}


#endif

// end of file
