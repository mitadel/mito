# -*- cmake -*-
#
# michael a.g. aïvázis <michael.aivazis@para-sim.com>
# (c) 1998-2020 all rights reserved
#

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
project(MITO LANGUAGES CXX C)
include(GNUInstallDirs)

# set up c++
mito_cxxInit()

# set up user configurable options
mito_optionsInit()

# initialize the variables that describe the install directory layout
mito_destinationInit()

# visit subdirectories
add_subdirectory(lib)
#add_subdirectory(packages)
#add_subdirectory(extensions)

# if we are building the test suite
if(BUILD_TESTING)
  # get support
  include(mito_tests)
  # set up the test suite
  mito_testsInit()
endif(BUILD_TESTING)

# end of file
