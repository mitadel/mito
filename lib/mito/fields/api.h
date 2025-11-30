// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fields {

    // uniform field
    template <geometry::coordinates_c coordType, class Y>
    constexpr auto uniform_field(const Y & constant);

    // the order N identity tensor in D dimensions
    template <geometry::coordinates_c coordType, int N>
    constexpr auto identity_tensor_field = uniform_field<coordType>(tensor::identity<N>);
}


// end of file
