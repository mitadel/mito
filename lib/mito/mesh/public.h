// code guard
#if !defined(mito_mesh_public_h)
#define mito_mesh_public_h


// external packages
#include "externals.h"

// get the forward declarations
#include "forward.h"

// published type factories; this is the file you are looking for...
#include "api.h"

// classes implementation
#include "Mesh.h"

// factories implementation
#include "factories.h"

// mesh utilities
#include "utilities.h"

#ifdef WITH_METIS
#include "metis/public.h"
#endif

#endif

// end of file
