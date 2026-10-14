// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// linear system implementation
#include "linear_system/public.h"

// backend implementation
#if WITH_PETSC
#include "petsc/public.h"
#endif    // WITH_PETSC


// end of file
