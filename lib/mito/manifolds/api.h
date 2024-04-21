// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::manifolds {

    // a point in parametric coordinates
    template <int D>
    using parametric_point_t = std::array<double, D>;

    // manifold alias
    template <class cellT, geometry::CoordinateType coordsT>
    using manifold_t = Manifold<cellT, coordsT>;

    // form alias
    template <int N, class F>
    using form_t = Form<N, F>;

    // one-form alias
    template <class F>
    using one_form_t = Form<1, F>;

    // field alias
    template <geometry::CoordinateType coordT, functions::function_c F>
    using field_t = Field<coordT, F>;

    // factory manifolds
    template <class cellT, geometry::CoordinateType coordsT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<cellT::dim, coordsT> & coordinate_system)
        -> manifold_t<cellT, coordsT>;

    // factory for one-forms
    template <class F>
    constexpr auto one_form(F && f) -> one_form_t<F>;

    // factory for P-forms
    template <int P, class F>
    constexpr auto form(F && f) -> form_t<P, F>;

    // factory for fields
    template <geometry::CoordinateType coordT, functions::function_c F>
    constexpr auto field(const F & f) -> field_t<coordT, F>;

    // factory for fields
    template <geometry::CoordinateType coordT, functions::function_c F>
    constexpr auto field(F && f) -> field_t<coordT, F>;

    // uniform field
    template <int D, geometry::CoordinateType coordsT = geometry::CARTESIAN, class Y>
    constexpr auto uniform_field(const Y & constant);

    // the order N identity tensor in D dimensions
    template <int N, int D, geometry::CoordinateType coordsT = geometry::CARTESIAN>
    constexpr auto identity_tensor_field = uniform_field<D, coordsT>(mito::identity<N>);

    // // TOFIX:
    // template <class F, class G, int D = field_t<F>::dim>
    // constexpr auto one_form(
    //     const field_t<F> & vector, const field_t<G> & metric = identity_tensor_field<D, D>)
    // requires(
    //     // the vector and the metric are defined on the same coordinates
    //     std::is_same_v<typename field_t<F>::coordinates_type, typename
    //     field_t<G>::coordinates_type>
    //     // {vector} is a vector field
    //     && vector_field_c<field_t<F>>
    //     // {metric} is a symmetric tensor field
    //     && symmetric_tensor_field_c<field_t<G>>);
}


#endif

// end of file
