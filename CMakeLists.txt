# -*- cmake -*-

# cmake setup
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

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

# additional compilation flags
set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -Wall -Wextra -pedantic -Werror")

# programs
find_program(BASH_PROGRAM bash)

# packages
# pyre
find_package(pyre REQUIRED)

# python
find_package(Python 3.10 COMPONENTS Interpreter Development NumPy)

# pybind11
set(PYBIND11_CPP_STANDARD -std=c++20)
set(PYBIND11_PYTHON_VERSION ${Python_VERSION})
set(PYBIND11_FINDPYTHON ON) # Use new FindPython if available
find_package(pybind11)

# pybind11 pre-2.5 C++17 compilation is broken under Clang
# see https://github.com/pybind/pybind11/issues/1604#issuecomment-443385783
if(pybind11_VERSION VERSION_LESS 2.5 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(pybind11::module INTERFACE -fsized-deallocation)
endif()

if(WITH_VTK)
    find_package(VTK REQUIRED COMPONENTS CommonCore CommonDataModel IOXML)
    message(STATUS "Enable VTK support")
    add_definitions(-DWITH_VTK)
endif()

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
    include(mito_tests_mito_ext)
endif()

# if we are building the benchmarks
if(MITO_BUILD_BENCHMARKS)
    # get my functions
    include(mito_benchmarks)

    # add the benchmarks to the pile
    include(mito_benchmarks_mito_lib)
endif()

# end of file
