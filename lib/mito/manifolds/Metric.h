// code guard
#if !defined(mito_manifold_Metric_h)
#define mito_manifold_Metric_h


namespace mito::manifolds {

    // the metric tensor field of type {metricT} for an N-dimensional manifold embedded in
    //  D-dimensional space
    template <metric_t metricT, int N, int D>
    class Metric {};

    // specialization for the Euclidean metric
    template <int N, int D>
    class Metric<EUCLIDEAN, N, D> {
      public:
        static constexpr auto metric() { return identity_tensor_field<N, D>; }
    };

    // specialization for the polar metric in 2D
    template <>
    class Metric<POLAR, 2, 2> {
      public:
        static constexpr auto metric()
        {
            return field(
                [](const mito::geometry::coordinates_t<2> & x) -> mito::diagonal_matrix_t<2> {
                    // e_rr + r^2 * e_tt
                    return mito::e_00<2> + (x[0] * x[0]) * mito::e_11<2>;
                });
        }
    };
}


#endif

// end of file
