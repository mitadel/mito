// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_coordinates_factories_h)
#define mito_coordinates_factories_h


namespace mito::geometry {

    // factory for coordinates from brace-enclosed initializer list
    template <coordinates_c coordT>
    constexpr auto coordinates(mito::scalar_t (&&coords)[coordT::dim])
    {
        return coordT(std::move(coords));
    }

}


#endif

// end of file
