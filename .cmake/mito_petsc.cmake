# -*- cmake -*-
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#


# PETSC support
option(WITH_PETSC "Enable support for PETSC solvers" OFF)

# if PETSC is requested
if(WITH_PETSC)
    # check if MPI is available
    if(NOT MPI_FOUND)
        # if MPI is not found, report an error (PETSc requires MPI)
        message(SEND_ERROR "PETSc requires MPI support, but MPI was not requested or found.")
    endif()
    # find PETSc
    find_package(PkgConfig REQUIRED)
    pkg_search_module(PETSC PETSc>3.5.0 IMPORTED_TARGET REQUIRED)
    # report
    message(STATUS "Enable PETSc ${PETSC_VERSION} support")
    # add compiler definitions
    add_definitions(-DWITH_PETSC)
    # report
    message(STATUS "Add definition WITH_PETSC")
    # include PETSc headers
    target_include_directories(mito SYSTEM PUBLIC ${PETSC_INCLUDE_DIRS})
    # link against PETSc library
    target_link_libraries(mito PUBLIC ${PETSC_LDFLAGS})
endif()


# end of file
