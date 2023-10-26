// code guard
#if !defined(mito_manifolds_api_h)
#define mito_manifolds_api_h


namespace mito::manifolds {

    // factory manifolds
    template <class cellT, int D>
    constexpr auto manifold(mesh::mesh_t<cellT, D> & mesh);

    // a point in parametric coordinates
    template <int D>
    using parametric_point_t = std::array<double, D>;

    // manifold alias
    template <class cellT, int D, class F>
    using manifold_t = Manifold<cellT, D, F>;

    // form alias
    template <int N, class F>
    using form_t = Form<N, F>;

    // one-form alias
    template <class F>
    using one_form_t = Form<1, F>;

    // field alias
    template <class F>
    using field_t = Field<F>;

    // uniform field
    template <int D, class Y>
    constexpr auto uniform_field(const Y & constant)
    {
        return field([constant](const mito::vector_t<D> &) -> Y { return constant; });
    }

    // the order N identity tensor in D dimensions
    template <int N, int D>
    constexpr auto identity_tensor_field = uniform_field<D>(mito::identity<N>);
}


#endif

// end of file
