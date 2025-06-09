# -*- cmake -*-
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#


# CUDA support
option(WITH_CUDA "Enable support for CUDA" OFF)

# if CUDA is requested
if(WITH_CUDA)
    # find CUDA package
    find_package(CUDAToolkit REQUIRED)
    # report
    message(STATUS "Enable CUDA support")
    # add compiler definitions
    add_definitions(-DWITH_CUDA)
    message(STATUS "Add definition WITH_CUDA")
    # enable CUDA language for targets
    enable_language(CUDA)
    # include CUDA headers
    target_include_directories(mito PUBLIC ${CUDAToolkit_INCLUDE_DIRS})
    # link against CUDA libraries
    target_link_libraries(mito PUBLIC CUDA::cudart CUDA::cusolver)
    # set CUDA host compiler to match the C++ compiler (to tell NVCC the compiler to use for the host code)
    if(CMAKE_CUDA_COMPILER_ID STREQUAL "NVIDIA")
        set(CMAKE_CUDA_HOST_COMPILER "${CMAKE_CXX_COMPILER}" CACHE FILEPATH "Host compiler for NVCC")
    endif()
    message(STATUS "Using CUDA host compiler: ${CMAKE_CUDA_HOST_COMPILER}")
endif()


# end of file
