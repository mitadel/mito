// code guard
#if !defined(mito_manifold_utilities_h)
#define mito_manifold_utilities_h


namespace mito::manifolds {

    // type trait for the dimension of the input of function {F}
    template <class F>
    struct input {
        // input of function {F}
        using X = typename mito::math::function_t<F>::X;
        // output of function {F}
        using Y = typename mito::math::function_t<F>::Y;
        // strip from {X} the cv-qualifiers and references, and get the size of the input (vector)
        static constexpr int dim = mito::utilities::base_type<X>::size;
    };

    // concept for a form {FORM} being a one-form on a D-dimensional manifold
    template <class FORM>
    // {FORM} is a one form if:
    concept is_one_form =
        // ... it takes in input a mito::vector_t<D>
        std::is_same_v<
            mito::utilities::base_type<typename input<typename FORM::function_type>::X>,
            mito::vector_t<FORM::dim>>
        // ... and returns a scalar
        && std::is_same_v<typename input<typename FORM::function_type>::Y, mito::scalar_t>;
}


#endif

// end of file
