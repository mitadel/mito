// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


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


// end of file
