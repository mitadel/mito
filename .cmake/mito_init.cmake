# -*- cmake -*-

# setup the user configurable options
function(mito_optionsInit)
    # disable build of test suite by default
    option(MITO_BUILD_TESTING "Build the MiTo test suite" OFF)

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
    set(CMAKE_CXX_STANDARD 20)

    # all done
endfunction(mito_cxxInit)

# describe the installation layout
function(mito_destinationInit)
    # create variables to hold the roots in the install directory
    set(MITO_DEST_INCLUDE ${CMAKE_INSTALL_INCLUDEDIR} PARENT_SCOPE)
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

    # all done
endfunction(mito_getVersion)

# end of file
