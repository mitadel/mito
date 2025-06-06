// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::cuda {

    // cuda dense solver
    auto dense(const std::string & name)
    {
        return dense_t(name);
    }

}


// end of file