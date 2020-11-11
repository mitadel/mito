# cmake setup
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# adjust the include path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/.cmake)

# get support
include(mito_init)

# set up the project
project(MITO LANGUAGES CXX C)

# set up c++
mito_cxxInit()

# set up user configurable options
mito_optionsInit()

# visit subdirectories
#add_subdirectory(lib)
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
