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
    struct metric<cartesian_coordinates_t<D>> {
        static constexpr auto field()
        {
            // return the identity field
            return fields::identity_tensor_field<cartesian_coordinates_t<D>, D>;
        }
    };
}


#endif

// end of file
