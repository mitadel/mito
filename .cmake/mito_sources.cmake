# -*- cmake -*-
#
# Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
#


# the mito version file
set(MITO_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/lib/mito/version.cc)

# the mito petsc backend
if (WITH_PETSC)
set(MITO_SOURCES ${MITO_SOURCES}
lib/mito/matrix_solvers/backend/petsc/PETScLinearSystem.cc
lib/mito/matrix_solvers/backend/petsc/PETScKrylovSolver.cc
)
endif()

# the mito cuda solvers backend
if (WITH_CUDA)
set(MITO_SOURCES ${MITO_SOURCES}
    lib/mito/solvers/backend/cuda/CUDASolver.cu
    lib/mito/solvers/backend/cuda/CUDADenseSolver.cu
    lib/mito/solvers/backend/cuda/CUDASparseSolver.cu
)
endif()


# end of file
