# -*- cmake -*-
#
# Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
#


# the mito version file
set(MITO_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/lib/mito/version.cc)

# the mito petsc backend
if (WITH_PETSC)
set(MITO_SOURCES ${MITO_SOURCES}
lib/mito/linalg/petsc/Matrix.cc
lib/mito/linalg/petsc/Vector.cc
lib/mito/linalg/petsc/LinearSystem.cc
lib/mito/linalg/petsc/KrylovSolver.cc
)
endif()


# end of file
