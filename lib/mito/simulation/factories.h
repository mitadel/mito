// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_simulation_factories_h)
#define mito_simulation_factories_h


namespace mito::simulation {

    // simulation factory
    auto simulation() -> simulation_t &
    {
        return utilities::Singleton<simulation_t>::GetInstance();
    }

}

#endif

// end of file
