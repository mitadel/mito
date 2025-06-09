# -*- cmake -*-
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#

# cmake setup
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# adjust the include path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/.cmake)

# get support
include(mito_init)
include(mito_mito)

# ask git for the mito version
mito_getVersion()

# set up the project
project(MITO VERSION "${MITO_VERSION}" LANGUAGES CXX)
set(MAJOR ${PROJECT_VERSION_MAJOR})
set(MINOR ${PROJECT_VERSION_MINOR})
set(MICRO ${PROJECT_VERSION_PATCH})

include(GNUInstallDirs)

# programs
find_program(BASH_PROGRAM bash)

# set up c++
mito_cxxInit()

# set up python
mito_pythonInit()

# set up user configurable options
mito_optionsInit()

# initialize the variables that describe the install directory layout
mito_destinationInit()

# visit subdirectories
include(mito_lib)
include(mito_extensions)

# mpi support
include(mito_mpi)

# pyre support
include(mito_pyre)

# vtk support
include(mito_vtk)

# metis support
include(mito_metis)

# petsc support
include(mito_petsc)

# cuda support
include(mito_cuda)

# configure mito as a cmake package (so that mito can be found by cmake with find_package(mito))
mito_shareCmakePackage()

# if we are building the test suite
if(MITO_BUILD_TESTING)
    # googletest
    find_package(GTest REQUIRED)

    # get support
    include(CTest)

    # and my functions
    include(mito_tests)

    # set up the test suite
    mito_testsInit()

    # add the mito lib testsuite to the pile
    include(mito_tests_mito_lib)

    # add the mito extensions testsuite to the pile
    # include(mito_tests_mito_ext) #TOFIX
endif()

# if we are building the benchmarks
if(MITO_BUILD_BENCHMARKS)
    # find the benchmark library
    find_package(benchmark REQUIRED)

    # get my functions
    include(mito_benchmarks)

    # add the benchmarks to the pile
    include(mito_benchmarks_mito_lib)
endif()

# end of file
