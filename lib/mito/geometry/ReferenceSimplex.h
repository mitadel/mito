// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    template <int N>
    class ReferenceSimplex {

      public:
        // the type of coordinates in the parametric space
        using parametric_coordinates_type = tensor::vector_t<N>;

        // the function extracting the I component of a parametric point
        template <int I>
        static constexpr auto xi = functions::component<parametric_coordinates_type, I>;
    };

}    // namespace mito


// end of file
