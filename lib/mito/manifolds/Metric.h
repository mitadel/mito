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
            // the function extracting the x_0 component of 2D vector
            constexpr auto x0 = mito::functions::x<0, 2>;
            // the function returning the constant e00 tensor in 2D
            constexpr auto e00 = mito::functions::constant<mito::vector_t<2>>(mito::e_00<2>);
            // the function returning the constant e11 tensor in 2D
            constexpr auto e11 = mito::functions::constant<mito::vector_t<2>>(mito::e_11<2>);

            // return the field e_rr + r^2 * e_tt
            return mito::manifolds::field<geometry::POLAR>(e00 + (x0 * x0) * e11);
        }
    };
}


#endif

// end of file
