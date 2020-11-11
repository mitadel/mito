# setup the user configurable options
function(mito_optionsInit)

  # disable build of test suite by default
  if(NOT BUILD_TESTING)
    set(BUILD_TESTING OFF CACHE STRING "Build the MiTo test suite")
  endif(NOT BUILD_TESTING)

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

  # check for highest c++ standard between c++11,..., c++17 and use it
  CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
  if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_STANDARD 17)
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++17")
  else()
    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    if(COMPILER_SUPPORTS_CXX14)
      set(CMAKE_CXX_STANDARD 14)
      message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++14")
    else()
      CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
      if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_STANDARD 11)
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports C++11")
      else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. " 
          "Please use a different C++ compiler.")
      endif()
    endif()
  endif()

  # all done
endfunction(mito_cxxInit)

# end of file
