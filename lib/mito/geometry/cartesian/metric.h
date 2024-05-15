// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_cartesian_Metric_h)
#define mito_geometry_cartesian_Metric_h


namespace mito::geometry {

    // specialization for the Euclidean metric
    template <int D>
    struct metric<coordinates_t<D, CARTESIAN>> {
        static constexpr auto field()
        {
            // the type of coordinates
            using coordinates_t = coordinates_t<D, CARTESIAN>;

            // return the identity field
            return fields::identity_tensor_field<coordinates_t, D>;
        }
    };
}


#endif

// end of file
