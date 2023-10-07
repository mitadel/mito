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

    // factory for one form
    template <class F>
    constexpr auto one_form(F && f)
    {
        return mito::manifolds::OneForm<F>(f);
    }

    // construct a one-form based on its metric-equivalent vector
    template <int D>
    constexpr auto one_form(mito::vector_t<D> vector)
    {
        // return a one-form that, when contracted with {x}...
        return one_form([vector](const mito::vector_t<D> & x) -> mito::scalar_t {
            // ... returns the contraction of {vector} with {x}
            return vector * x;
        });
    }

}


#endif

// end of file
