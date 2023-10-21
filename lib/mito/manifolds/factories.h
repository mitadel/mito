// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // TOFIX
    // factory manifolds
    template <class cellT, int D>
    constexpr auto manifold(mesh::mesh_t<cellT, D> & mesh)
    {
        return manifold_t<cellT, D>(mesh);
    }

    // factory for one-forms
    template <class F>
    constexpr auto one_form(F && f)
    {
        return mito::manifolds::one_form_t<F>(f);
    }

    // factory for tensors
    template <class F>
    constexpr auto tensor(F && f)
    {
        return mito::manifolds::tensor_t<F>(f);
    }

    // construct a one-form based on its metric-equivalent vector (case: symmetric metric)
    template <int D>
    constexpr auto one_form(
        const mito::vector_t<D> & vector, const mito::symmetric_matrix_t<D> & inverse_metric)
    {
        // return a one-form that, when contracted with {v}...
        return one_form([vector, inverse_metric](const mito::vector_t<D> & v) -> mito::scalar_t {
            // ... returns the contraction of {inverse_metric} with {vector} and {v}
            return inverse_metric * vector * v;
        });
    }

    // construct a one-form based on its metric-equivalent vector (case: diagonal metric)
    template <int D>
    constexpr auto one_form(
        const mito::vector_t<D> & vector, const mito::diagonal_matrix_t<D> & inverse_metric)
    {
        // return a one-form that, when contracted with {v}...
        return one_form([vector, inverse_metric](const mito::vector_t<D> & v) -> mito::scalar_t {
            // ... returns the contraction of {inverse_metric} with {vector} and {v}
            return inverse_metric * vector * v;
        });
    }
    // construct a one-form based on its metric-equivalent vector
    template <class F, class G, int D = field_t<F>::dim>
    constexpr auto one_form(field_t<F> vector, field_t<G> metric)
    requires(
        // the vector and the metric are define on the same vector space
        field_t<F>::dim == field_t<G>::dim
        // {vector} is a vector field
        && is_vector_field<field_t<F>>
        // {metric} is a symmetric tensor field
        && is_symmetric_tensor_field<field_t<G>>)
    {
        // return a one-form that, when contracted with {x}...
        return one_form([vector, metric](const mito::vector_t<D> & x) -> mito::scalar_t {
            // ... returns the contraction of {vector} with {x}
            return metric(x) * vector(x) * x;
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
}


#endif

// end of file
