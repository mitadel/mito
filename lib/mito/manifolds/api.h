// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::manifolds {

    // a point in parametric coordinates
    template <int D>
    using parametric_point_t = std::array<double, D>;

    // manifold alias
    template <geometry::CoordinateType coordsT, class cellT, int D>
    using manifold_t = Manifold<coordsT, cellT, D>;

    // form alias
    template <int N, class F>
    using form_t = Form<N, F>;

    // one-form alias
    template <class F>
    using one_form_t = Form<1, F>;

    // field alias
    template <class F>
    using field_t = Field<F>;

    // factory manifolds
    template <geometry::CoordinateType coordsT = geometry::EUCLIDEAN, class cellT, int D>
    constexpr auto manifold(
        const mesh::mesh_t<cellT, D> & mesh,
        const geometry::coordinate_system_t<D, coordsT> & coordinate_system)
        -> manifold_t<coordsT, cellT, D>;

    // factory for one-forms
    template <class F>
    constexpr auto one_form(F && f) -> one_form_t<F>;

    // factory for P-forms
    template <int P, class F>
    constexpr auto form(F && f) -> form_t<P, F>;

    // factory for fields
    template <class F>
    constexpr auto field(F && f) -> field_t<F>;

    // uniform field
    template <int D, geometry::CoordinateType coordsT = geometry::EUCLIDEAN, class Y>
    constexpr auto uniform_field(const Y & constant)
    {
        return field(
            [constant](const geometry::coordinates_t<D, coordsT> &) -> Y { return constant; });
    }

    // the order N identity tensor in D dimensions
    template <int N, int D, geometry::CoordinateType coordsT = geometry::EUCLIDEAN>
    constexpr auto identity_tensor_field = uniform_field<D, coordsT>(mito::identity<N>);

    // TOFIX:
    template <class F, class G, int D = field_t<F>::dim>
    constexpr auto one_form(
        const field_t<F> & vector, const field_t<G> & metric = identity_tensor_field<D, D>)
    requires(
        // the vector and the metric are defined on the same coordinates
        std::is_same_v<typename field_t<F>::coordinates_type, typename field_t<G>::coordinates_type>
        // {vector} is a vector field
        && VectorField<field_t<F>>
        // {metric} is a symmetric tensor field
        && SymmetricTensorField<field_t<G>>);
}


#endif

// end of file
