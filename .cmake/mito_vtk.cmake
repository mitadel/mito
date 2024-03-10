# -*- cmake -*-
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#


# VTK support
option(WITH_VTK "Enable support for VTK" OFF)

# if VTK is requested
if(WITH_VTK)
    # find VTK
    find_package(VTK REQUIRED COMPONENTS CommonCore CommonDataModel IOXML)
    # report
    message(STATUS "Enable VTK support")
    # add compiler definitions
    add_definitions(-DWITH_VTK)
    # include VTK headers
    target_include_directories(mito PUBLIC ${VTK_INCLUDE_DIRS})
    # link against VTK libraries
    target_link_libraries(mito ${VTK_LIBRARIES})
endif()


# end of file
