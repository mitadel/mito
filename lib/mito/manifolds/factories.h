// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // factory manifolds
    template <class cellT, geometry::CoordinateType coordsT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<cellT::dim, coordsT> & coordinate_system)
        -> manifold_t<cellT, coordsT>
    {
        return manifold_t<cellT, coordsT>(mesh, coordinate_system);
    }

    // factory for one-forms
    template <class F>
    constexpr auto one_form(F && f) -> one_form_t<F>
    {
        return one_form_t<F>(std::forward<F>(f));
    }

    // factory for P-forms
    template <int P, class F>
    constexpr auto form(F && f) -> form_t<P, F>
    {
        return form_t<P, F>(std::forward<F>(f));
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
    template <functions::function_c F>
    constexpr auto field(const F & f) -> mito::manifolds::field_t<F>
    {
        return mito::manifolds::field_t<F>(f);
    }

    // factory for fields
    template <functions::function_c F>
    constexpr auto field(F && f) -> mito::manifolds::field_t<F>
    {
        return mito::manifolds::field_t<F>(std::forward<F>(f));
    }

    // factory for fields (from functors)
    template <functions::functor_c F>
    constexpr auto field(F && f)
    {
        return mito::manifolds::field(mito::functions::function(std::move(f)));
    }

    // uniform field
    template <geometry::coordinates_c coordType, class Y>
    constexpr auto uniform_field(const Y & constant)
    {
        return field(mito::functions::constant<coordType>(constant));
    }

    // construct a one-form based on its metric-equivalent vector field
    template <vector_field_c vectorFieldT, symmetric_tensor_field_c tensorFieldT>
    constexpr auto one_form(const vectorFieldT & vector, const tensorFieldT & metric)
    requires(compatible_fields_c<vectorFieldT, tensorFieldT>)
    {
        // the coordinate type
        using coordinates_t = typename vectorFieldT::coordinates_type;

        // return a one-form that, when contracted with {x}...
        return mito::manifolds::field(
            mito::functions::function([vector, metric](const coordinates_t & x) {
                // ... returns the contraction of {vector} with {x}
                return mito::manifolds::one_form(vector(x), metric(x));
            }));
    }
}


#endif

// end of file
