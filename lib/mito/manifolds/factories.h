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
    template <int D>
    constexpr auto one_form(
        mito::vector_t<D> vector, mito::symmetric_matrix_t<D> metric = mito::identity<D>)
    {
        // return a one-form that, when contracted with {x}...
        return form([vector, metric](const mito::vector_t<D> & x) -> mito::scalar_t {
            // ... returns the contraction of {vector} with {x}
            return metric * vector * x;
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
