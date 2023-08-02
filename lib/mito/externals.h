// code guard
#if !defined(mito_externals_h)
#define mito_externals_h


// externals
#include <array>
#include <vector>
#include <iostream>

#define HAVE_TENSOR
#define HAVE_COMPACT_PACKINGS
#include <pyre/tensor.h>

#ifdef WITH_MPI
#include <mpi.h>
#endif

#endif

// end of file
