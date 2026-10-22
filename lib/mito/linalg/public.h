// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once

// get the concepts
#include "concepts.h"

// class implementations
#include "BlockSegment.h"
#include "linear_system.h"

// published types factories; this is the file you are looking for...
#include "api.h"

// backend implementations
#if WITH_PETSC
#include "petsc.h"
#endif    // WITH_PETSC


// end of file
