// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::simulation {

    // simulation factory
    auto simulation() -> simulation_t &
    {
        return utilities::Singleton<simulation_t>::GetInstance();
    }

}


// end of file
