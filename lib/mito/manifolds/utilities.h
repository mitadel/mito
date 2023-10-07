// code guard
#if !defined(mito_manifold_utilities_h)
#define mito_manifold_utilities_h


namespace mito::manifolds {

    // concept for a function {F} being a one forms on a D-dimensional manifold
    template <class FUNCTION, int D>
    // {F} is a one form if:
    concept is_one_form =
        // ... it takes in input a mito::vector_t<D>
        std::is_same<mito::utilities::base_type<typename FUNCTION::X>, mito::vector_t<D>>::value
        // ... and returns a scalar
        && std::is_same<typename FUNCTION::Y, mito::scalar_t>::value;

    // type trait for the dimension of the input of function {F}
    template <class F>
    struct input {
        static constexpr int dim =
            mito::utilities::base_type<typename mito::math::function_t<F>::X>::size;
    };

}


#endif

// end of file
