# -*- cmake -*-
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
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


# end of file
