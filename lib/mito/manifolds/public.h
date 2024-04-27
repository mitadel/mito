// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_public_h)
#define mito_manifolds_public_h


// external packages
#include "externals.h"

// get the forward declarations
#include "forward.h"

// published type factories; this is the file you are looking for...
#include "api.h"

// classes implementation
#include "parametric.h"
#include "Form.h"
#include "Field.h"

// factories implementation
#include "factories.h"

// arithmetic operations on fields
#include "fields_arithmetic.h"
// algebraic operations on fields
#include "fields_algebra.h"
// algebraic operations on forms
#include "forms_algebra.h"
// wedge operator on forms
#include "wedge.h"
// tensor operator on forms
#include "tensor.h"

// more classes implementation
#include "Metric.h"
#include "Manifold.h"

#endif

// end of file
