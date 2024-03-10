# -*- cmake -*-
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#


# METIS support
option(WITH_METIS "Enable support for Metis" OFF)

# if METIS is requested
if(WITH_METIS)
    # find METIS
    find_package(METIS REQUIRED)
    # report
    message(STATUS "Enable Metis support")
    # add compiler definitions
    add_definitions(-DWITH_METIS)
    # include METIS headers
    include_directories(${METIS_INCLUDE_DIRS})
    # link against METIS libraries
    target_link_libraries(mito ${METIS_LIBRARIES})
endif()


# end of file
