// code guard
#if !defined(mito_manifold_forms_h)
#define mito_manifold_forms_h


namespace mito::manifolds {

    // construct a one-form based on its metric-equivalent vector
    template <int D>
    constexpr auto one_form(mito::vector_t<D> vector)
    {
        // return the contraction of a given vector {x} with {vector}
        return mito::math::function([vector](const mito::vector_t<D> & x) -> mito::scalar_t {
            // return the contraction of {vector} with {x}
            return vector * x;
        });
    }

    // concept for a function {F} being a one forms on a D-dimensional manifold
    template <int D, class F>
    // {F} is a one form if:
    concept is_one_form =
        // ... it takes in input a mito::vector_t<D>
        std::is_same<
            mito::utilities::base_type<typename mito::math::function_t<F>::X>,
            mito::vector_t<D>>::value
        // ... and returns a scalar
        && std::is_same<typename mito::math::function_t<F>::Y, mito::scalar_t>::value;
}


#endif

// end of file
