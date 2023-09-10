# -*- cmake -*-

# setup the test suite
function(mito_testsInit)
    # enable test target
    enable_testing()

    # print test output in case of failure
    set($ENV{CTEST_OUTPUT_ON_FAILURE} 1)

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
        string(REPLACE ";" "_" args "_${ARGN}")
    endif()

    # assemble the test name and hand it off
    set(${testcase} "${stem}${args}" PARENT_SCOPE)

    # all done
endfunction()

function(mito_test_target targetname filename)
    # generate the name of the target
    mito_target_name(target ${testfile})

    # hand off {targetname} to parent scope
    set(${targetname} "${target}" PARENT_SCOPE)

    # schedule it to be compiled
    add_executable(${target} ${testfile})

    # with some macros
    target_compile_definitions(${target} PRIVATE MITO_CORE)

    # link against my libraries
    target_link_libraries(${target} PUBLIC mito)

    # link against gtest
    target_link_libraries(${target} PUBLIC GTest::gtest_main)

    # specify the directory for the target compilation products
    mito_target_directory(${target} tests)

endfunction(mito_test_target)

# register a test case based on a compiled driver
function(mito_test_driver testfile)
    # generate the name of the testcase
    mito_test_testcase(testname ${testfile} ${ARGN})

    # configure target for this test
    mito_test_target(target ${testfile})

    # setup the test working directory
    set(test_workdir ${CMAKE_CURRENT_SOURCE_DIR}/tests/input)

    # make it a test case
    add_test(NAME ${testname} COMMAND ${target} ${ARGN} WORKING_DIRECTORY ${test_workdir})

    # register the runtime environment requirements
    set_property(TEST ${testname} PROPERTY ENVIRONMENT
        LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
    )

    # all done
endfunction(mito_test_driver)

# register a parallel test case based on a compiled driver
function(mito_test_driver_mpi testfile slots)
    # generate the name of the testcase (append the number of MPI slots requested)
    mito_test_testcase(testname ${testfile} ${ARGN} ${slots})

    # configure target for this test
    mito_test_target(target ${testfile})

    # setup the test working directory
    set(test_workdir ${CMAKE_CURRENT_SOURCE_DIR}/tests/input)

    # make it a test case
    add_test(NAME ${testname} COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${slots}
        ${MPIEXEC_PREFLAGS} $<TARGET_FILE:${target}> ${MPIEXEC_POSTFLAGS} ${ARGN}
        WORKING_DIRECTORY ${test_workdir})

    # register the runtime environment requirements
    set_property(TEST ${testname} PROPERTY ENVIRONMENT
        LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
    )

  # all done
endfunction()

# register a test case based on a compiled driver which produces some output and another test
# case based on pytest to check the generated output
function(mito_test_driver_pytest_check testfile)
    # register the test
    mito_test_driver(${testfile})

    # generate the name of the target for the test
    mito_target_name(targetTest ${testfile})

    # generate the name of the target for checking the output
    string(APPEND targetScript "${targetTest}.pytest")

    # assemble the full path to the python file that does the check
    get_filename_component(dir ${testfile} DIRECTORY)
    file(REAL_PATH ${dir} dir)
    get_filename_component(file ${testfile} NAME_WE)
    set(filepath "${dir}/${file}.py")

    # setup the test working directory
    set(test_workdir ${CMAKE_CURRENT_SOURCE_DIR}/tests/input)

    # make a test case that checks the output
    add_test(NAME ${targetScript}
        COMMAND ${BASH_PROGRAM} -c "pytest -v ${filepath}" WORKING_DIRECTORY ${test_workdir}
    )

    # specify the order of execution
    set_property(TEST ${targetScript} PROPERTY
        DEPENDS ${targetTest}
    )

    # register the runtime environment requirements
    set_property(TEST ${targetScript} PROPERTY ENVIRONMENT
        PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/tests/scripts
    )

endfunction(mito_test_driver_pytest_check)

# register a python script as a test case; use a path relative to {PROJECT_SOURCE_DIR}
function(mito_test_python_testcase testfile)
    # generate the name of the testcase
    mito_test_testcase(testname ${testfile} ${ARGN})

    # get the absolute path to the file
    file(REAL_PATH ${testfile} testfile)

    # setup the test working directory
    set(test_workdir ${CMAKE_CURRENT_SOURCE_DIR}/tests/input)

    # set up the harness
    add_test(NAME ${testname}
        COMMAND ${Python_EXECUTABLE} ${testfile} ${ARGN} WORKING_DIRECTORY ${test_workdir})

    # register the runtime environment requirements
    set_property(TEST ${testname} PROPERTY ENVIRONMENT
        PYTHONPATH=${MITO_DEST_FULL_EXTENSIONS}
    )

    # all done
endfunction()

# end of file
