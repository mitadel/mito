// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#pragma once


namespace mito::tensor {

    // empty class for dummy vector for contraction
    class dummy_vector {};

    // instance of dummy vector for contraction
    static constexpr dummy_vector _;


    // concept of an extended vector (vector or dummy vector)
    template <class F>
    concept vector_or_dummy_c = vector_c<F> or std::is_same_v<F, dummy_vector>;


    // class for P-forms
    template <int P, class F>
    class Form {

      public:
        // alias for my type
        using form_type = Form<P, F>;
        // alias for my underlying function type
        using function_type = F;

      public:
        // constructor
        constexpr Form(F f) : _f{ std::move(f) } {}

        // contraction operator
        template <vector_or_dummy_c... argsT>
        constexpr auto operator()(argsT... args) const
        requires(sizeof...(argsT) == P)
        {
            return _f(args...);
        }

      private:
        // the action of the form
        F _f;
    };


    // specialization for one-forms
    template <class F>
    class Form<1, F> {
      public:
        // alias for my type
        using form_type = Form<1, F>;
        // alias for my underlying function type
        using function_type = F;

      public:
        // constructor
        constexpr Form(F f) : _f{ f } {}

        // contraction with a vector
        template <class X>
        constexpr auto operator()(const X & x) const -> mito::scalar_t
        requires(!std::is_same_v<X, dummy_vector>)
        {
            return _f(x);
        }

        // contraction with a dummy vector (do not perform contraction)
        constexpr auto operator()(const dummy_vector &) const -> const form_type & { return *this; }

      private:
        // the action of the one-form
        F _f;
    };
}


// end of file
