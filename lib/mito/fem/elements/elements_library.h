// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <geometry::geometric_simplex_c cellT, int DEGREE>
    struct finite_element_family;

}

#include "seg1/public.h"
#include "tri1/public.h"
#include "tri2/public.h"


// end of file
