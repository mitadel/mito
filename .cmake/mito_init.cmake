# -*- cmake -*-
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#


# setup the user configurable options
function(mito_optionsInit)
    # disable build of test suite by default
    option(MITO_BUILD_TESTING "Build the MiTo test suite" OFF)

    # disable build of the benchmarks by defaults
    option(MITO_BUILD_BENCHMARKS "Build MiTo benchmarks" OFF)

    # list possible types of build
    set(CMAKE_BUILD_TYPES Debug Release RelWithDebInfo)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_BUILD_TYPES})

    # build debug library by default
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
    endif(NOT CMAKE_BUILD_TYPE)

    # all done
endfunction(mito_optionsInit)

# setup the c++ compiler
function(mito_cxxInit)
    # set C++ standard
    include(CheckCXXCompilerFlag)

    # request c++20
    set(CMAKE_CXX_STANDARD 20 PARENT_SCOPE)
    set(CMAKE_CXX_STANDARD_REQUIRED True PARENT_SCOPE)

    # additional compilation flags
    set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -Wall -Wextra -pedantic -Werror" PARENT_SCOPE)

    # all done
endfunction(mito_cxxInit)

# describe the installation layout
function(mito_destinationInit)
    # create variables to hold the roots in the install directory
    set(MITO_DEST_INCLUDE ${CMAKE_INSTALL_INCLUDEDIR} PARENT_SCOPE)

    if(NOT DEFINED MITO_DEST_EXTENSIONS)
        set(MITO_DEST_EXTENSIONS extensions CACHE STRING
            "Python package install location, absolute or relative to install prefix")
    endif()

    # Translate to unconditional absolute path
    get_filename_component(MITO_DEST_FULL_EXTENSIONS ${MITO_DEST_EXTENSIONS} ABSOLUTE
        BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
    set(MITO_DEST_FULL_EXTENSIONS ${MITO_DEST_FULL_EXTENSIONS} PARENT_SCOPE)
endfunction(mito_destinationInit)

# ask git for the most recent tag and use it to build the version
function(mito_getVersion)
    # git
    find_package(Git REQUIRED)

    # get tag info
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --long --always
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_DESCRIBE_STATUS
        OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # the parser of the "git describe" result
    set(GIT_DESCRIBE "v([0-9]+)\\.([0-9]+)\\.([0-9]+)-([0-9]+)-g(.+)")

    if(GIT_DESCRIBE_VERSION MATCHES ${GIT_DESCRIBE})
        # parse the bits
        string(REGEX REPLACE ${GIT_DESCRIBE} "\\1" REPO_MAJOR ${GIT_DESCRIBE_VERSION})
        string(REGEX REPLACE ${GIT_DESCRIBE} "\\2" REPO_MINOR ${GIT_DESCRIBE_VERSION})
        string(REGEX REPLACE ${GIT_DESCRIBE} "\\3" REPO_MICRO ${GIT_DESCRIBE_VERSION})
        string(REGEX REPLACE ${GIT_DESCRIBE} "\\5" REPO_COMMIT ${GIT_DESCRIBE_VERSION})
    else()
        set(REPO_MAJOR 1)
        set(REPO_MINOR 0)
        set(REPO_MICRO 0)
        set(REPO_COMMIT ${GIT_DESCRIBE_VERSION})
    endif()

    # export our local variables
    set(REPO_MAJOR ${REPO_MAJOR} PARENT_SCOPE)
    set(REPO_MINOR ${REPO_MINOR} PARENT_SCOPE)
    set(REPO_MICRO ${REPO_MICRO} PARENT_SCOPE)
    set(REPO_COMMIT ${REPO_COMMIT} PARENT_SCOPE)

    # set the variables used in the package meta-data files
    set(MAJOR ${REPO_MAJOR} PARENT_SCOPE)
    set(MINOR ${REPO_MINOR} PARENT_SCOPE)
    set(MICRO ${REPO_MICRO} PARENT_SCOPE)
    set(REVISION ${REPO_COMMIT} PARENT_SCOPE)
    string(TIMESTAMP TODAY PARENT_SCOPE)

    set(MITO_VERSION "${REPO_MAJOR}.${REPO_MINOR}.${REPO_MICRO}" PARENT_SCOPE)

    # all done
endfunction(mito_getVersion)

function(mito_shareCmakePackage)
    # make exports available in binary dir during build
    export(EXPORT mito-targets
        NAMESPACE mito::
    )

    # install exports to installation prefix
    set(MITO_CMAKE_DIR "share/cmake/mito" CACHE STRING
        "Installation directory for cmake files, relative to install prefix")
    install(EXPORT mito-targets
        NAMESPACE mito::
        DESTINATION ${MITO_CMAKE_DIR}
    )

    # set up version detection for cmake find_package
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        mito-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY ExactVersion
    )

    # install config file for find_package
    configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/.cmake/mito-config.cmake.in
        ${PROJECT_BINARY_DIR}/mito-config.cmake
        INSTALL_DESTINATION ${MITO_CMAKE_DIR})
    install(FILES ${PROJECT_BINARY_DIR}/mito-config.cmake
        ${PROJECT_BINARY_DIR}/mito-config-version.cmake
        DESTINATION ${MITO_CMAKE_DIR})
    # copy the {FindMETIS.cmake}
    install(FILES ${PROJECT_SOURCE_DIR}/.cmake/FindMETIS.cmake
            DESTINATION ${MITO_CMAKE_DIR})

    # create aliases matching the exports above
    add_library(mito::mito ALIAS mito)

    # all done
endfunction(mito_shareCmakePackage)

# set up python
function(mito_pythonInit)

    # python
    find_package(Python 3.10 COMPONENTS Interpreter Development NumPy)

    # pybind11
    set(PYBIND11_PYTHON_VERSION ${Python_VERSION})
    set(PYBIND11_FINDPYTHON ON) # Use new FindPython if available
    find_package(pybind11)

    # pybind11 pre-2.5 C++17 compilation is broken under Clang
    # see https://github.com/pybind/pybind11/issues/1604#issuecomment-443385783
    if(pybind11_VERSION VERSION_LESS 2.5 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(pybind11::module INTERFACE -fsized-deallocation)
    endif()

    # ask the executable for the module suffix
    execute_process(
        COMMAND ${Python_EXECUTABLE} -c
        "from sysconfig import *; print(get_config_var('EXT_SUFFIX'))"
        RESULT_VARIABLE PYTHON3_SUFFIX_STATUS
        OUTPUT_VARIABLE PYTHON3_SUFFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # export
    set(PYTHON3_SUFFIX ${PYTHON3_SUFFIX} PARENT_SCOPE)
    set(Python_EXECUTABLE ${Python_EXECUTABLE} PARENT_SCOPE)

    # all done
endfunction(mito_pythonInit)

# end of file
