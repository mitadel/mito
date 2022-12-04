// code guard
#if !defined(mito_topology_public_h)
#define mito_topology_public_h


// external packages
#include "externals.h"

// get the forward declarations
#include "forward1.h"
// class {OrientedSimplex} needs to be complete for the following forward declarations
// (the concept {ReferenceCountedObject} requires to see full class header file)
#include "OrientedSimplex.h"
// get more forward declarations
#include "forward2.h"
// other classes implementation
#include "Simplex.h"
// simplex utilities
#include "utilities.h"
#include "SimplexFactory.h"
#include "OrientedSimplexFactory.h"
#include "Topology.h"

// published type factories; this is the file you are looking for...
#include "api.h"

// factories implementation
#include "factories.h"

#endif

// end of file
