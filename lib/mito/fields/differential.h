// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_fields_differential_h)
#define mito_fields_differential_h


// Differential operators on Fields
namespace mito::fields {

    // the {I...}-th first partial derivative of a field
    template <int... I>
    constexpr auto derivative(const field_c auto & f)
    {
        // the {I...}-th first partial derivative
        return field(mito::functions::derivative<I...>(f.function()));
    }

    // function to compute the gradient of a scalar field
    template <scalar_field_c F>
    constexpr auto gradient(const F & field)
    {
        // the type of coordinate
        using coordinate_t = F::coordinates_type;
        // the spatial dimension of the field
        constexpr int D = coordinate_t::dim;

        // helper function to compute the gradient of a scalar
        constexpr auto _grad = []<size_t... I>(const F & field, std::index_sequence<I...>) {
            // the vector of the partial derivatives
            return ((derivative<I>(field) * uniform_field<coordinate_t>(e<I, D>)) + ...);
        };

        return _grad(field, std::make_index_sequence<D>{});
    }
}


#endif

// end of file
