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
    # disable offline compilation support warnings
    target_compile_options(mito PUBLIC $<$<COMPILE_LANGUAGE:CUDA>:-Wno-deprecated-gpu-targets>)
    # find Eigen for CUDA sparse solver
    find_package(Eigen3 3.3 QUIET NO_MODULE)
    if(Eigen3_FOUND)
        # add Eigen compiler definition
        message(STATUS "Found Eigen3: ${EIGEN3_INCLUDE_DIR}")
        add_definitions(-DWITH_EIGEN)
        # add include directories for Eigen
        target_include_directories(mito PUBLIC ${EIGEN3_INCLUDE_DIR})
        # disable warnings due to incompatible return types in Eigen
        target_compile_options(mito PUBLIC $<$<COMPILE_LANGUAGE:CUDA>:--expt-relaxed-constexpr>)
        # find cudss library
        find_package(cudss REQUIRED)
        # include cudss headers
        target_include_directories(mito PUBLIC ${cudss_INCLUDE_DIR})
        # link against cudss libraries
        target_link_libraries(mito PUBLIC cudss)
    else()
        message(WARNING "Eigen3 not found. CUDA sparse solver parts will be disabled.")
    endif()
endif()


# end of file
