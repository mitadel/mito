// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// external packages
#include "externals.h"

// get the forward declarations
#include "forward1.h"
// published type factories; this is the file you are looking for...
#include "api.h"
// get more forward declarations
#include "forward2.h"
// other classes implementation
#include "Simplex.h"
// class {OrientedSimplex} needs to be complete for the following forward declarations
// (the concept {reference_countable_c} requires to see full class header file)
#include "OrientedSimplex.h"
#include "SimplexFactory.h"
#include "OrientedSimplexFactory.h"
#include "Topology.h"

// factories implementation
#include "factories.h"

// simplex utilities
#include "utilities.h"

// support for flip diagonal
#include "FlipDiagonal.h"


// end of file
