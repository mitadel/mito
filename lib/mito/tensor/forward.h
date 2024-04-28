// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_tensor_forward_h)
#define mito_tensor_forward_h


namespace mito::tensor {

    // class P-form
    template <int P, class F>
    class Form;

    // concept of a one-form
    template <class F>
    concept one_form_c = requires(F c) {
        // require that F only binds to {one_form_t} specializations
        []<class T>(const Form<1, T> &) {
        }(c);
    };

    // concept of a p-form
    template <class F>
    concept p_form_c = requires(F c) {
        // require that F only binds to {p_form_t} specializations
        []<int P, class T>(const Form<P, T> &) {
        }(c);
    };

}


#endif

// end of file
