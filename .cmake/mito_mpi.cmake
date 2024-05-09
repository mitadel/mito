# -*- cmake -*-
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
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
    add_definitions(-DWITH_MPI)
    # include MPI headers
    target_include_directories(mito PUBLIC ${MPI_INCLUDE_PATH})
    # link against MPI libraries
    target_link_libraries(mito MPI::MPI_CXX)
endif()


# end of file
