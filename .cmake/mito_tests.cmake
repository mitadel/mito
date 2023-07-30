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

# register a test case based on a compiled driver
function(mito_test_driver testfile)
    # generate the name of the testcase
    mito_test_testcase(testname ${testfile} ${ARGN})

    # generate the name of the target
    mito_target_name(target ${testfile})

    # schedule it to be compiled
    add_executable(${target} ${testfile})

    # with some macros
    target_compile_definitions(${target} PRIVATE MITO_CORE)

    # link against my libraries
    target_link_libraries(${target} PUBLIC mito)

    # link against gtest
    target_link_libraries(${target} PUBLIC GTest::gtest_main)

    # setup the test working directory
    get_filename_component(path ${testfile} DIRECTORY)
    set(test_workdir ${CMAKE_CURRENT_SOURCE_DIR}/${path})

    # make it a test case
    add_test(NAME ${testname} COMMAND ${target} ${ARGN} WORKING_DIRECTORY ${test_workdir})

    # specify the directory for the target compilation products
    mito_target_directory(${target} tests)

    # register the runtime environment requirements
    set_property(TEST ${testname} PROPERTY ENVIRONMENT
        LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
    )

    # request c++20
    set_property(TARGET ${target} PROPERTY CXX_STANDARD 20)

    # all done
endfunction(mito_test_driver)

# register a parallel test case based on a compiled driver
function(mito_test_driver_mpi testfile slots)
    # generate the name of the testcase (append the number of MPI slots requested)
    mito_test_testcase(testname ${testfile} ${ARGN} ${slots})

    # generate the name of the target
    mito_target_name(target ${testfile})

    # schedule it to be compiled
    add_executable(${target} ${testfile})

    # with some macros
    target_compile_definitions(${target} PRIVATE MITO_CORE WITH_MPI)

    # link against my libraries
    target_link_libraries(${target} PUBLIC mito)

    # link against gtest and MPI
    target_link_libraries(${target} PUBLIC GTest::gtest_main)

    # setup the test working directory
    get_filename_component(path ${testfile} DIRECTORY)
    set(test_workdir ${CMAKE_CURRENT_SOURCE_DIR}/${path})

    # make it a test case
    add_test(NAME ${testname} COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${slots}
        ${MPIEXEC_PREFLAGS} $<TARGET_FILE:${target}> ${MPIEXEC_POSTFLAGS} ${ARGN}
        WORKING_DIRECTORY ${test_workdir})

    # specify the directory for the target compilation products
    mito_target_directory(${target} tests)

    # register the runtime environment requirements
    set_property(TEST ${testname} PROPERTY ENVIRONMENT
        LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
    )

    # request c++20
    set_property(TARGET ${target} PROPERTY CXX_STANDARD 20)

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

    # get the filename
    get_filename_component(fileName ${testfile} NAME_WE)

    # get the test directory
    get_filename_component(path ${testfile} DIRECTORY)
    set(test_workdir ${CMAKE_CURRENT_SOURCE_DIR}/${path})

    # make a test case that checks the output
    add_test(NAME ${targetScript}
        COMMAND ${BASH_PROGRAM} -c "pytest -v ${fileName}.py" WORKING_DIRECTORY ${test_workdir}
    )

    # specify the order of execution
    set_property(TEST ${targetScript} PROPERTY
        DEPENDS ${targetTest}
    )
endfunction(mito_test_driver_pytest_check)

# register a python script as a test case; use a path relative to {PROJECT_SOURCE_DIR}
function(mito_test_python_testcase testfile)
    # generate the name of the testcase
    mito_test_testcase(testname ${testfile} ${ARGN})

    # we run the test cases in their local directory, so we need the base name
    get_filename_component(base ${testfile} NAME)

    # get the relative path to the test case local directory so we can set the working dir
    get_filename_component(dir ${testfile} DIRECTORY)

    # set up the harness
    add_test(NAME ${testname}
        COMMAND ${Python_EXECUTABLE} ./${base} ${ARGN})

    # register the runtime environment requirements
    set_property(TEST ${testname} PROPERTY ENVIRONMENT
        PYTHONPATH=${MITO_DEST_FULL_EXTENSIONS}
    )

    # launch from the location of the testcase
    set_property(TEST ${testname} PROPERTY
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/${dir}
    )

    # all done
endfunction()

# end of file
