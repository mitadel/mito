// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // factory for coordinates from brace-enclosed initializer list
    template <coordinates_c coordT>
    constexpr auto coordinates(mito::tensor::scalar_t (&&coords)[coordT::dim])
    {
        return coordT(std::move(coords));
    }

}


// end of file
