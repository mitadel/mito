// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


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


// end of file
