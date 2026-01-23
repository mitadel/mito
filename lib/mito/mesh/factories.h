// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::mesh {

    // mesh factory
    template <class cellT>
    auto mesh() -> mesh_t<cellT>
    {
        return mesh_t<cellT>();
    }

}


// end of file
