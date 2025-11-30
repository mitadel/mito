// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fields {

    // uniform field
    template <geometry::coordinates_c coordType, class Y>
    constexpr auto uniform_field(const Y & constant)
    {
        return mito::functions::constant<coordType>(constant);
    }
}


// end of file
