// code guard
#if !defined(mito_math_fields_algebra_h)
#define mito_math_fields_algebra_h


// Algebraic operations on Fields
namespace mito::manifolds {

    // f^(-1)
    template <class F>
    constexpr auto inverse(const field_t<F> & f)
    {
        constexpr int D = field_t<F>::dim;
        return function([f](const mito::vector_t<D> & x) { return inverse(f(x)); });
    }

    // det(f)
    template <class F>
    constexpr auto determinant(const field_t<F> & f)
    {
        constexpr int D = field_t<F>::dim;
        return field([f](const mito::vector_t<D> & x) { return determinant(f(x)); });
    }

    // sqrt(f)
    template <class F>
    constexpr auto sqrt(const field_t<F> & f)
    {
        constexpr int D = field_t<F>::dim;
        return field([f](const mito::vector_t<D> & x) { return std::sqrt(f(x)); });
    }
}


#endif

// end of file
