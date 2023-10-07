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
    template <int D, class FUNCTION>
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

    // the wedge product of two forms
    template <class F1, class F2, int D = input<F1>::dim>
    constexpr auto wedge(
        const mito::math::function_t<F1> & a_tilda, const mito::math::function_t<F2> & b_tilda)
    requires(
        is_one_form<D, mito::math::function_t<F1>> && is_one_form<D, mito::math::function_t<F2>>)
    {
        // return the contraction of a given vector {x} with {vector}
        return mito::math::function([a_tilda, b_tilda](const mito::vector_t<D> & x) -> auto {
            // return the contraction of {vector} with {x}
            return a_tilda(x) * b_tilda - b_tilda(x) * a_tilda;
        });
    }

}


#endif

// end of file
