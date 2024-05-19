// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_public_h)
#define mito_geometry_public_h


// external packages
#include "externals.h"

// get the forward declarations
#include "forward.h"

// published type factories; this is the file you are looking for...
#include "api.h"

// support
#include "basis.h"
#include "metric.h"
#include "metric_space.h"

// algebra
#include "algebra_coordinates.h"
#include "coordinate_transformation.h"

// coordinate specialization
#include "cartesian.h"
#include "polar.h"
#include "spherical.h"

// classes implementation
#include "CoordinateSystem.h"
#include "Point.h"
#include "GeometricSimplex.h"
#include "PointCloud.h"

// factories implementation
#include "factories.h"

// geometry utilities
#include "utilities.h"

#endif

// end of file
