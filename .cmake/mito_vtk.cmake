# -*- cmake -*-
#
# Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
#


# VTK support
option(WITH_VTK "Enable support for VTK" OFF)

# if VTK is requested
if(WITH_VTK)
    # if MPI is also requested
    if(WITH_MPI)
        # the {ParallelMPI} vtk component requires the C language
        enable_language(C)
        # try to find VTK with the Parallel components too
        find_package(VTK COMPONENTS CommonCore CommonDataModel IOXML IOParallelXML ParallelMPI)
        # if VTK was found
        if(VTK_FOUND)
            # report
            message(STATUS "Enable VTK support")
            # add compiler definitions
            add_definitions(-DWITH_VTK)
            # report
            message(STATUS "Added definition WITH_VTK")
            # add compiler definitions
            add_definitions(-DWITH_PARALLEL_VTK)
            # record that VTK is available in its parallel flavor
            set(WITH_PARALLEL_VTK TRUE)
            # report
            message(STATUS "Add definition WITH_PARALLEL_VTK")
            # include VTK headers
            target_include_directories(mito SYSTEM PUBLIC ${VTK_INCLUDE_DIRS})
            # link against VTK libraries
            target_link_libraries(mito PUBLIC ${VTK_LIBRARIES})
        endif()
    endif()
    # if VTK was not found yet
    if(NOT VTK_FOUND)
        # find the essential components of VTK
        find_package(VTK REQUIRED COMPONENTS CommonCore CommonDataModel IOXML)
        # report
        message(STATUS "Enable VTK support")
        # add compiler definitions
        add_definitions(-DWITH_VTK)
        # report
        message(STATUS "Add definition WITH_VTK")
        # include VTK headers
        target_include_directories(mito SYSTEM PUBLIC ${VTK_INCLUDE_DIRS})
        # link against VTK libraries
        target_link_libraries(mito PUBLIC ${VTK_LIBRARIES})
    endif()
endif()


# end of file
