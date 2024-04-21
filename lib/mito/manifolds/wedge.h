// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_wedge_h)
#define mito_manifolds_wedge_h


// wedge operator on forms
namespace mito::manifolds {

    // the wedge product of one one-form (trivial case)
    constexpr auto wedge(const one_form_c auto & a_tilda)
    {
        // return a {a_tilda}
        return a_tilda;
    }

    // the wedge product of two one-forms
    constexpr auto wedge(const one_form_c auto & a_tilda, const one_form_c auto & b_tilda)
    {
        // return a {form} that, when contracted with {x} and {y}...
        return mito::manifolds::form<2>(
            [a_tilda, b_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y>(
                const X & x, const Y & y) -> auto {
                // ... returns the {scalar} prescribed by the wedge product
                return a_tilda(x) * b_tilda(y) - b_tilda(x) * a_tilda(y);
            });
    }

    // the wedge product of three one-forms
    constexpr auto wedge(
        const one_form_c auto & a_tilda, const one_form_c auto & b_tilda,
        const one_form_c auto & c_tilda)
    {
        // return a {form} that, when contracted with {x}, {y} and {z}...
        return mito::manifolds::form<3>(
            [a_tilda, b_tilda,
             c_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y, vector_or_dummy_c Z>(
                const X & x, const Y & y, const Z & z) -> auto {
                // ... returns the {scalar} prescribed by the wedge product
                return a_tilda(x) * wedge(b_tilda, c_tilda)(y, z)
                     + b_tilda(x) * wedge(c_tilda, a_tilda)(y, z)
                     + c_tilda(x) * wedge(a_tilda, b_tilda)(y, z);
            });
    }

    // the wedge product of one field of one-forms (trivial case)
    constexpr auto wedge(const one_form_field_c auto & fA)
    {
        // return a {fA}
        return fA;
    }

    // the wedge product of two fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2>
    constexpr auto wedge(const F1 & fA, const F2 & fB)
    requires(
        // {fA} and {fB} are defined on the same coordinates
        std::is_same_v<typename F1::coordinates_type, typename F2::coordinates_type>)
    {
        using coordinates_type = typename F1::coordinates_type;
        return field<F1::coordinate_type>(functions::function(
            [fA, fB](const coordinates_type & x) { return wedge(fA(x), fB(x)); }));
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
        return field<F1::coordinate_type>(functions::function(
            [fA, fB, fC](const coordinates_type & x) { return wedge(fA(x), fB(x), fC(x)); }));
    }

}


#endif

// end of file
