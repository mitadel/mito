// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_tensor_api_h)
#define mito_tensor_api_h


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

}


#endif

// end of file
