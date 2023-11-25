// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::manifolds {

    // a point in parametric coordinates
    template <int D>
    using parametric_point_t = std::array<double, D>;

    // manifold alias
    template <geometry::CoordinateSystem coordsT, class cellT, int D>
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
    template <geometry::CoordinateSystem coordsT = geometry::EUCLIDEAN, class cellT, int D>
    constexpr auto manifold(const mesh::mesh_t<cellT, D> & mesh) -> manifold_t<coordsT, cellT, D>;

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
    template <int D, class Y>
    constexpr auto uniform_field(const Y & constant)
    {
        return field([constant](const mito::vector_t<D> &) -> Y { return constant; });
    }

    // the order N identity tensor in D dimensions
    template <int N, int D>
    constexpr auto identity_tensor_field = uniform_field<D>(mito::identity<N>);

    template <class F, class G, int D = field_t<F>::dim>
    constexpr auto one_form(
        const field_t<F> & vector, const field_t<G> & metric = identity_tensor_field<D, D>)
    requires(
        // the vector and the metric are define on the same vector space
        field_t<F>::dim == field_t<G>::dim
        // {vector} is a vector field
        && VectorField<field_t<F>>
        // {metric} is a symmetric tensor field
        && SymmetricTensorField<field_t<G>>);
}


#endif

// end of file
