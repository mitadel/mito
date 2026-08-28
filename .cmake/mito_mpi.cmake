# -*- cmake -*-
#
# Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
#


# MPI support
option(WITH_MPI "Enable support for MPI" ON)

# if MPI is requested
if(WITH_MPI)
    # mpi
    find_package(MPI COMPONENTS CXX REQUIRED)
    # report
    message(STATUS "Enable MPI support")
    # add compiler definitions
    target_compile_definitions(mito PUBLIC WITH_MPI)
    # link against MPI libraries
    target_link_libraries(mito PUBLIC MPI::MPI_CXX)
endif()


# end of file
