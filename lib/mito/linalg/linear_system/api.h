// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg {

    // linear system
    template <valid_backend_c backendT>
    using linear_system_t = utilities::NamedClass<LinearSystem<backendT>>;
}


// end of file
