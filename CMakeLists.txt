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

# packages
# pyre
find_package(pyre REQUIRED)

# set up c++
mito_cxxInit()

# set up user configurable options
mito_optionsInit()

# initialize the variables that describe the install directory layout
mito_destinationInit()

# visit subdirectories
add_subdirectory(lib)

# add_subdirectory(packages)
# add_subdirectory(extensions)

# configure mito as a cmake package (so that mito can be found by cmake with find_package(mito))
mito_shareCmakePackage()

# if we are building the test suite
if(MITO_BUILD_TESTING)
    # get support
    include(CTest)

    # and my functions
    include(mito_tests)

    # set up the test suite
    mito_testsInit()

    # add the testsuite to the pile
    add_subdirectory(tests)
endif(MITO_BUILD_TESTING)

# end of file
