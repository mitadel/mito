// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_fields_factories_h)
#define mito_fields_factories_h


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

    // construct a one-form based on its metric-equivalent vector field
    template <vector_field_c vectorFieldT, symmetric_tensor_field_c tensorFieldT>
    constexpr auto one_form_field(const vectorFieldT & vector, const tensorFieldT & metric)
    requires(compatible_fields_c<vectorFieldT, tensorFieldT>)
    {
        // the coordinate type
        using coordinates_t = typename vectorFieldT::coordinates_type;

        // return a one-form that, when contracted with {x}...
        return field(functions::function([vector, metric](const coordinates_t & x) {
            // ... returns the contraction of {vector} with {x}
            return tensor::one_form(vector(x), metric(x));
        }));
    }
}


#endif

// end of file
