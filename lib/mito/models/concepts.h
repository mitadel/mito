// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::models {

    // concept of a thermal material
    template <class M>
    concept thermal_material_c = requires(const M & m) {
        { m.conductivity() } -> fields::tensor_field_c;
        { m.heat_capacity() } -> fields::scalar_field_c;
        { m.heat_source() } -> fields::scalar_field_c;
    };
}


// end of file
