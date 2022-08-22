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
project(MITO LANGUAGES CXX)
include(GNUInstallDirs)

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
