// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::tensor {

    // form alias
    template <int N, class F>
    using form_t = Form<N, F>;

    // one-form alias
    template <class F>
    using one_form_t = Form<1, F>;

    // factory for one-forms
    template <class F>
    constexpr auto one_form(F && f) -> one_form_t<F>;

    // factory for P-forms
    template <int P, class F>
    constexpr auto form(F && f) -> form_t<P, F>;

    // the tensor product of two one-forms
    constexpr auto tensor(const one_form_c auto & a_tilda, const one_form_c auto & b_tilda);

    // the tensor product of three one-forms
    constexpr auto tensor(
        const one_form_c auto & a_tilda, const one_form_c auto & b_tilda,
        const one_form_c auto & c_tilda);

    // the wedge product of two one-forms
    constexpr auto wedge(const one_form_c auto & a_tilda, const one_form_c auto & b_tilda);

    // the wedge product of three one-forms
    constexpr auto wedge(
        const one_form_c auto & a_tilda, const one_form_c auto & b_tilda,
        const one_form_c auto & c_tilda);
}


// end of file
