// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito {

    // remove the {pyre} prefix
    using namespace pyre::tensor;

    // typedef for scalars
    using scalar_t = real;

    // concept of a scalar
    template <class F>
    concept scalar_c = std::convertible_to<F, scalar_t>;

    // concept for a tensor (vector, matrix, higher-order, ...) or a scalar
    template <class F>
    concept tensor_or_scalar_c = tensor_c<F> or scalar_c<F>;

    // I-th basis vector in dimension D
    template <int I, int D>
    requires(D > 0 && I >= 0 && I < D)
    constexpr auto e = unit<vector_t<D>, I>;

    template <int D>
    constexpr auto e_0 = e<0, D>;

    template <int D>
    constexpr auto e_1 = e<1, D>;

    template <int D>
    constexpr auto e_2 = e<2, D>;

    template <int D>
    requires(D > 0)
    constexpr auto e_00 = unit<matrix_t<D>, 0, 0>;

    template <int D>
    requires(D > 1)
    constexpr auto e_01 = unit<matrix_t<D>, 0, 1>;

    template <int D>
    requires(D > 1)
    constexpr auto e_10 = unit<matrix_t<D>, 1, 0>;

    template <int D>
    requires(D > 1)
    constexpr auto e_11 = unit<matrix_t<D>, 1, 1>;

    template <int D>
    requires(D > 2)
    constexpr auto e_02 = unit<matrix_t<D>, 0, 2>;

    template <int D>
    requires(D > 2)
    constexpr auto e_20 = unit<matrix_t<D>, 2, 0>;

    template <int D>
    requires(D > 2)
    constexpr auto e_12 = unit<matrix_t<D>, 1, 2>;

    template <int D>
    requires(D > 2)
    constexpr auto e_21 = unit<matrix_t<D>, 2, 1>;

    template <int D>
    requires(D > 2)
    constexpr auto e_22 = unit<matrix_t<D>, 2, 2>;

    template <int D>
    requires(D > 0)
    constexpr auto identity = pyre::tensor::identity<matrix_t<D>>;

}


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
