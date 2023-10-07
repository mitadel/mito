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

}


#endif

// end of file
