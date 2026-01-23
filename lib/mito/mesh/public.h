// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// external packages
#include "externals.h"

// get the forward declarations
#include "forward.h"

// published type factories; this is the file you are looking for...
#include "api.h"

// classes implementation
#include "Mesh.h"
#include "Boundary.h"
#include "Filter.h"

// factories implementation
#include "factories.h"

// mesh utilities
#include "utilities.h"

#ifdef WITH_METIS
#include "metis/public.h"
#endif


// end of file
