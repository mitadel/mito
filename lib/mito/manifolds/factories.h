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
    template <geometry::CoordinateType coordT, functions::function_c F>
    constexpr auto field(const F & f) -> mito::manifolds::field_t<coordT, F>
    {
        return mito::manifolds::field_t<coordT, F>(f);
    }

    // factory for fields
    template <geometry::CoordinateType coordT, functions::function_c F>
    constexpr auto field(F && f) -> mito::manifolds::field_t<coordT, F>
    {
        return mito::manifolds::field_t<coordT, F>(std::forward<F>(f));
    }

    // uniform field
    template <int D, geometry::CoordinateType coordsT, class Y>
    constexpr auto uniform_field(const Y & constant)
    {
        using coordinates_type = geometry::coordinates_t<D, coordsT>;
        return field<coordsT>(mito::functions::constant<coordinates_type>(constant));
    }

    // // construct a one-form based on its metric-equivalent vector field
    // template <class F, class G, int D>
    // constexpr auto one_form(const field_t<F> & vector, const field_t<G> & metric)
    // requires(
    //     // the vector and the metric are defined on the same coordinates
    //     std::is_same_v<typename field_t<F>::coordinates_type, typename
    //     field_t<G>::coordinates_type>
    //     // {vector} is a vector field
    //     && VectorField<field_t<F>>
    //     // {metric} is a symmetric tensor field
    //     && SymmetricTensorField<field_t<G>>)
    // {
    //     // return a one-form that, when contracted with {x}...
    //     return field([vector, metric](const field_t<F>::coordinates_type & x) -> auto {
    //         // ... returns the contraction of {vector} with {x}
    //         return one_form(vector(x), metric(x));
    //     });
    // }
}


#endif

// end of file
