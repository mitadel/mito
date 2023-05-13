# -*- cmake -*-


# register a benchmark case based on a compiled driver
function(mito_benchmark_driver benchmarkfile)

    # generate the name of the target
    mito_target_name(target ${benchmarkfile})

    # schedule it to be compiled
    add_executable(${target} ${benchmarkfile})

    # in release mode
    target_compile_options(${target} PRIVATE "-O3")

    # with some macros
    target_compile_definitions(${target} PRIVATE MITO_CORE)

    # link against my libraries
    target_link_libraries(${target} PUBLIC mito)

    # link against pyre
    target_link_libraries(${target} PUBLIC pyre::pyre)

    # link against pyre
    target_link_libraries(${target} PUBLIC pyre::journal)

    # specify the directory for the target compilation products
    mito_target_directory(${target} benchmarks)

    # register the runtime environment requirements
    set_property(TEST ${testname} PROPERTY ENVIRONMENT
        LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
    )

    # request c++20
    set_property(TARGET ${target} PROPERTY CXX_STANDARD 20)

  # all done
endfunction()


# end of file