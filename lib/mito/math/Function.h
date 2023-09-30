
// code guard
#if !defined(mito_math_Function_h)
#define mito_math_Function_h


namespace mito::math {

    // We need a class function to explicitly put the return value Y in the template
    template <class X, class Y>
    class Function : public std::function<Y(X)> {};
}

#endif    // mito_math_Function_h

// end of file
