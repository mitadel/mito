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
    template <class cellT, int D>
    using manifold_t = Manifold<cellT, D>;

    // form alias
    template <class F>
    using one_form_t = OneForm<F>;

    // tensor alias
    template <class F>
    using tensor_t = Tensor<F>;

    // field alias
    template <class F>
    using field_t = Field<F>;
}


#endif

// end of file
