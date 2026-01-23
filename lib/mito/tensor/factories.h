// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::tensor {

    // factory for one-forms
    template <class F>
    constexpr auto one_form(F && f) -> one_form_t<F>
    {
        return one_form_t<F>(std::forward<F>(f));
    }

    // factory for P-forms
    template <int P, class F>
    constexpr auto form(F && f) -> form_t<P, F>
    {
        return form_t<P, F>(std::forward<F>(f));
    }

    // construct a one-form that, given a vector {vector} and a (symmetric) matrix {matrix},
    // operates on vectors by contracting them with {matrix} and {vector}
    template <int D>
    constexpr auto one_form(const vector_t<D> & vector, const symmetric_matrix_t<D> & matrix)
    {
        // return a one-form that, when contracted with {v}...
        return one_form([vector, matrix](const vector_t<D> & v) -> scalar_t {
            // ... returns the contraction of {matrix} with {vector} and {v}
            return matrix * vector * v;
        });
    }

    // construct a one-form that, given a vector {vector} and a (diagonal) matrix {matrix},
    // operates on vectors by contracting them with {matrix} and {vector}
    template <int D>
    constexpr auto one_form(
        const vector_t<D> & vector, const diagonal_matrix_t<D> & matrix = identity<D>)
    {
        // return a one-form that, when contracted with {v}...
        return one_form([vector, matrix](const vector_t<D> & v) -> scalar_t {
            // ... returns the contraction of {matrix} with {vector} and {v}
            return matrix * vector * v;
        });
    }
}


// end of file
