// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// traits on mito types
namespace mito {
    template <class T>
    constexpr int rank()
    {
        return T::rank;
    }

    template <>
    constexpr int rank<scalar_t>()
    {
        return 1;
    }

    template <class T>
    constexpr int size()
    {
        return T::size;
    }

    template <>
    constexpr int size<scalar_t>()
    {
        return 1;
    }

}


// end of file
