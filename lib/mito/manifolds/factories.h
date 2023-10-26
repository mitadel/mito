// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // factory manifolds
    template <class cellT, int D, class F>
    constexpr auto manifold(
        mesh::mesh_t<cellT, D> & mesh,
        const field_t<F> & metric = uniform_field<D>(mito::identity<D>))
    {
        return manifold_t<cellT, D, F>(mesh, metric);
    }

    // factory for one-forms
    template <class F>
    constexpr auto one_form(F && f)
    {
        return mito::manifolds::one_form_t<F>(f);
    }

    // factory for general forms
    template <int N, class F>
    constexpr auto form(F && f)
    {
        return mito::manifolds::form_t<N, F>(f);
    }

    // construct a one-form based on its metric-equivalent vector (case: symmetric metric)
    template <int D>
    constexpr auto one_form(
        const mito::vector_t<D> & vector, const mito::symmetric_matrix_t<D> & metric)
    {
        // return a one-form that, when contracted with {v}...
        return one_form([vector, metric](const mito::vector_t<D> & v) -> mito::scalar_t {
            // ... returns the contraction of {metric} with {vector} and {v}
            return metric * vector * v;
        });
    }

    // construct a one-form based on its metric-equivalent vector (case: diagonal metric)
    template <int D>
    constexpr auto one_form(
        const mito::vector_t<D> & vector,
        const mito::diagonal_matrix_t<D> & metric = mito::identity<D>)
    {
        // return a one-form that, when contracted with {v}...
        return one_form([vector, metric](const mito::vector_t<D> & v) -> mito::scalar_t {
            // ... returns the contraction of {metric} with {vector} and {v}
            return metric * vector * v;
        });
    }

    // factory for fields
    template <class F>
    constexpr auto field(F && f)
    {
        return mito::manifolds::field_t<F>(f);
    }

    // uniform field
    template <int D, class Y>
    constexpr auto uniform_field(const Y & constant)
    {
        return field([constant](const mito::vector_t<D> &) -> Y { return constant; });
    }

    // construct a one-form based on its metric-equivalent vector field
    template <class F, class G, int D = field_t<F>::dim>
    constexpr auto one_form(
        const field_t<F> & vector, const field_t<G> & metric = uniform_field<D>(mito::identity<D>))
    requires(
        // the vector and the metric are define on the same vector space
        field_t<F>::dim == field_t<G>::dim
        // {vector} is a vector field
        && is_vector_field<field_t<F>>
        // {metric} is a symmetric tensor field
        && is_symmetric_tensor_field<field_t<G>>)
    {
        // return a one-form that, when contracted with {x}...
        return field([vector, metric](const mito::geometry::coordinates_t<D> & x) -> auto {
            // ... returns the contraction of {vector} with {x}
            return one_form(vector(x), metric(x));
        });
    }
}


#endif

// end of file
