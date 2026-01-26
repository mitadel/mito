// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // localize the fem field {f} on {e}
    template <class elementT>
    auto localize(const fem_field_c auto & f, const elementT & e)
    {
        return f.localize(e);
    }

    // localize the field {f} on {e}
    template <class elementT>
    auto localize(const fields::field_c auto & f, const elementT & e)
    {
        return f(e.parametrization());
    }
}


// end of file
