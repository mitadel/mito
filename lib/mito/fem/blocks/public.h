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

// published types factories; this is the file you are looking for...
#include "api.h"

// utilities (must be included before blocks that use jacobian_measure)
#include "utilities.h"

// classes implementation
#include "AssemblyBlock.h"
#include "GradGradBlock.h"
#include "MassBlock.h"
#include "SourceTermBlock.h"
#include "L2NormBlock.h"

// factories implementation
#include "factories.h"

// end of file
