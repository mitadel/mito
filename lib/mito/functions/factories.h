// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::functions {

    // returns the function mapping an input of type {X} to the constant {c}
    template <class X, class Y>
    constexpr auto constant(const Y & c)
    {
        return Constant<X, Y>(c);
    }

    // returns the function wrapping the functor {f}
    template <functor_c F>
    constexpr auto function(F && f)
    {
        return FunctionFromFunctor<F>(std::forward<F>(f));
    }

    // returns the linear combination
    template <typename T, function_c... Funcs>
    constexpr auto linear_combination(std::array<T, sizeof...(Funcs)> coeffs, Funcs... funcs)
    {
        return LinearCombination<T, Funcs...>(coeffs, std::forward<Funcs>(funcs)...);
    }
}


// end of file
