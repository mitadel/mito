// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // factory manifolds
    template <geometry::CoordinateType coordsT, class cellT, int D>
    constexpr auto manifold(const mesh::mesh_t<cellT, D, coordsT> & mesh)
        -> manifold_t<coordsT, cellT, D>
    {
        return manifold_t<coordsT, cellT, D>(mesh);
    }

    // factory for one-forms
    template <class F>
    constexpr auto one_form(F && f) -> one_form_t<F>
    {
        return one_form_t<F>(f);
    }

    // factory for P-forms
    template <int P, class F>
    constexpr auto form(F && f) -> form_t<P, F>
    {
        return form_t<P, F>(f);
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
    constexpr auto field(F && f) -> mito::manifolds::field_t<F>
    {
        return mito::manifolds::field_t<F>(f);
    }

    // construct a one-form based on its metric-equivalent vector field
    template <class F, class G, int D>
    constexpr auto one_form(const field_t<F> & vector, const field_t<G> & metric)
    requires(
        // the vector and the metric are defined on the same coordinates
        std::is_same_v<typename field_t<F>::coordinates_type, typename field_t<G>::coordinates_type>
        // {vector} is a vector field
        && VectorField<field_t<F>>
        // {metric} is a symmetric tensor field
        && SymmetricTensorField<field_t<G>>)
    {
        // return a one-form that, when contracted with {x}...
        return field([vector, metric](const field_t<F>::coordinates_type & x) -> auto {
            // ... returns the contraction of {vector} with {x}
            return one_form(vector(x), metric(x));
        });
    }
}


#endif

// end of file
