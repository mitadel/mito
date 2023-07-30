# -*- cmake -*-


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
    include_directories(${MPI_INCLUDE_PATH})
    # link against MPI libraries
    target_link_libraries(mito MPI::MPI_CXX)
endif()


# end of file
