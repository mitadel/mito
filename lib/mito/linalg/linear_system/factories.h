// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg {

    // linear system
    template <valid_backend_c backendT>
    inline auto linear_system(const std::string & name, typename backendT::index_type size)
    {
        return linear_system_t<backendT>(name, size);
    }
}


// end of file
