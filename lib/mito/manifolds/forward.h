// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class manifold
    template <class cellT, int D>
    class Manifold;

    // manifold alias
    template <class cellT, int D>
    using manifold_t = Manifold<cellT, D>;

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

    // TOFIX: remove this datastructure
    // element vector alias
    template <class elementT>
    using element_vector_t = topology::element_vector_t<elementT>;
}


#endif

// end of file
