// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifold_Metric_h)
#define mito_manifold_Metric_h


namespace mito::manifolds {

    // the metric tensor field of type {coordsT} for an N-dimensional manifold embedded in
    //  D-dimensional space
    template <geometry::CoordinateType coordsT, int N, int D>
    struct metric {};

    // specialization for the Euclidean metric
    template <int N, int D>
    struct metric<geometry::CARTESIAN, N, D> {
        static constexpr auto field() { return identity_tensor_field<N, D, geometry::CARTESIAN>; }
    };

    // specialization for the polar metric in 2D
    template <>
    struct metric<geometry::POLAR, 2, 2> {
        static constexpr auto field()
        {
            return mito::manifolds::field(
                [](const mito::geometry::coordinates_t<2, geometry::POLAR> & x)
                    -> mito::diagonal_matrix_t<2> {
                    // e_rr + r^2 * e_tt
                    return mito::e_00<2> + (x[0] * x[0]) * mito::e_11<2>;
                });
        }
    };
}


#endif

// end of file
