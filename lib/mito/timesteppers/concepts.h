// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // an archetype of a first-order system M du/dt + K u = f
    template <class archetypeT>
    concept parabolic_archetype_c = std::same_as<archetypeT, models::ParabolicPDE>;
}


// end of file
