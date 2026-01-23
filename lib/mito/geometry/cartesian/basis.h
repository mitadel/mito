// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // cartesian coordinate basis in dimension D
    template <int D>
    struct basis<cartesian_coordinates_t<D>> {

      public:
        // the coordinate basis for vector fields
        template <int I>
        requires(I >= 0 && I < D)
        static constexpr auto e()
        {
            return functions::constant<cartesian_coordinates_t<D>>(tensor::e<I, D>);
        }
    };
}


// end of file
