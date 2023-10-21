// code guard
#if !defined(mito_manifold_tensors_h)
#define mito_manifold_tensors_h


namespace mito::manifolds {

    // addition of tensors fa + fb
    template <class F1, class F2>
    constexpr auto operator+(const tensor_t<F1> & fA, const tensor_t<F2> & fB)
    {
        return tensor(
            [fA, fB]<typename... argsT>(argsT... args) { return fA(args...) + fB(args...); });
    }

    // scalar * tensor
    template <class F>
    constexpr auto operator*(const real & a, const tensor_t<F> & f)
    {
        return tensor([a, f]<typename... argsT>(argsT... args) { return a * f(args...); });
    }

    // tensor * scalar
    template <class F>
    constexpr auto operator*(const tensor_t<F> & f, const real & a)
    {
        return a * f;
    }

    // unary operator- for tensors
    template <class F>
    constexpr auto operator-(const tensor_t<F> & f)
    {
        return -1.0 * f;
    }

    // subtraction of tensors fa - fb
    template <class F1, class F2>
    constexpr auto operator-(const tensor_t<F1> & fA, const tensor_t<F2> & fB)
    {
        return fA + (-fB);
    }
}


#endif

// end of file
