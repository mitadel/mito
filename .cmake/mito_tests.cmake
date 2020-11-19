# -*- cmake -*-
#
# michael a.g. aïvázis <michael.aivazis@para-sim.com>
# (c) 1998-2020 all rights reserved
#

# setup the test suite
function(mito_testsInit)
  # create a variable to hold the root in the test directory
  set(MITO_TESTSUITE_DIR ${CMAKE_SOURCE_DIR}/tests PARENT_SCOPE)
  set(MITO_TESTSUITE_TMPDIR ${CMAKE_BINARY_DIR}/tmp PARENT_SCOPE)
  # get support
  include(CTest)
  # enable test target 
  enable_testing()
  # print test output in case of failure
  set($ENV{CTEST_OUTPUT_ON_FAILURE} 1)
  # add the testsuite to the pile
  #add_subdirectory(tests)
  # all done
endfunction(mito_testsInit)


# generate a unique test case name that incorporate the command line arguments
# adapted from code by @rtburns-jpl
function(mito_test_testcase testcase testfile)
  # the test case stem is its path with path separators replaced by dots
  string(REPLACE "/" "." stem ${testfile})

  # initialize the command line argument pile
  set(args "")
  # if there are command line arguments
  if(ARGN)
    # mangle them
    string(REPLACE ";" "_"  args "_${ARGN}")
  endif()

  # assemble the test name and hand it off
  set(${testcase} "${stem}${args}" PARENT_SCOPE)

  # all done
endfunction()


# generate a unique test target name
function(mito_test_target target testfile)
  # split
  get_filename_component(path ${testfile} DIRECTORY)
  get_filename_component(base ${testfile} NAME_WE)

  # replace path separators with dors
  string(REPLACE "/" "." stem ${path})

  # buld the target and return it
  set(${target} "tests.${stem}.${base}" PARENT_SCOPE)

  # all done
endfunction()


# register a test case based on a compiled driver
function(mito_test_driver testfile)
  # generate the name of the testcase
  mito_test_testcase(testname ${testfile} ${ARGN})
  # generate the name of the target
  mito_test_target(target ${testfile})

  # schedule it to be compiled
  add_executable(${target} ${testfile})
  # with some macros
  target_compile_definitions(${target} PRIVATE MITO_CORE)
  # link against my libraries
  target_link_libraries(${target} PUBLIC mito)

  # make it a test case
  add_test(NAME ${testname} COMMAND ${target} ${ARGN})
  # register the runtime environment requirements
  set_property(TEST ${testname} PROPERTY ENVIRONMENT
    LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
    )

  # all done
endfunction(mito_test_driver)


# end of file
