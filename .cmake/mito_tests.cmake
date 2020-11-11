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

# end of file
