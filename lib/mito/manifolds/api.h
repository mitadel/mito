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
    template <functions::function_c F>
    using field_t = Field<F>;

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
    template <functions::function_c F>
    constexpr auto field(const F & f) -> field_t<F>;

    // factory for fields
    template <functions::function_c F>
    constexpr auto field(F && f) -> field_t<F>;

    // uniform field
    template <geometry::coordinates_c coordType, class Y>
    constexpr auto uniform_field(const Y & constant);

    // the order N identity tensor in D dimensions
    template <geometry::coordinates_c coordType, int N>
    constexpr auto identity_tensor_field = uniform_field<coordType>(mito::identity<N>);

    // the function associating to a D-dimensional set of coordinates its N-th coordinate
    template <class T, int N>
    [[maybe_unused]] constexpr auto coordinate = mito::functions::component<T, N>;

    // construct a one-form based on its metric-equivalent vector field
    template <vector_field_c vectorFieldT, symmetric_tensor_field_c tensorFieldT>
    constexpr auto one_form(const vectorFieldT & vector, const tensorFieldT & metric)
    requires(compatible_fields_c<vectorFieldT, tensorFieldT>);

}


#endif

// end of file
