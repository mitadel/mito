// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// external packages
#include "externals.h"

// get the concept declarations
#include "concepts.h"

// get the forward declarations
#include "forward.h"

// published types factories; this is the file you are looking for...
#include "api.h"

// backend implementation
#if WITH_PETSC
#include "petsc/public.h"
#endif    // WITH_PETSC

// classes implementation
#include "LinearSystem.h"

// factories implementation
#include "factories.h"


// end of file
