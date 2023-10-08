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

    // factory for forms
    template <class F>
    constexpr auto form(F && f)
    {
        return mito::manifolds::form_t<F>(f);
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
        return form([vector, metric](const mito::vector_t<D> & x) -> mito::scalar_t {
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
