# -*- cmake -*-

# build the mito libraries
function(mito_mitoLib)
    # build the libmito version file
    configure_file(
        lib/mito/version.h.in lib/mito/version.h
        @ONLY
    )
    configure_file(
        lib/mito/version.cc.in lib/mito/version.cc
        @ONLY
    )

    # copy the mito headers over to the staging area
    file(GLOB_RECURSE files
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/lib/mito
        CONFIGURE_DEPENDS
        lib/mito/*.h lib/mito/*.icc
    )

    foreach(file ${files})
        configure_file(lib/mito/${file} lib/mito/${file} COPYONLY)
    endforeach()

    # the libmito target
    add_library(mito SHARED)

    # specify the directory for the library compilation products
    mito_library_directory(mito lib)

    # set the include directories
    target_include_directories(
        mito PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:${MITO_DEST_INCLUDE}>
    )

    # add the sources
    target_sources(mito
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/lib/mito/version.cc
    )

    # and the link dependencies
    target_link_libraries(
        mito
    )

    # install all the mito headers
    install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/mito
        DESTINATION ${MITO_DEST_INCLUDE}
        FILES_MATCHING PATTERN *.h PATTERN *.icc
        PATTERN version.cc
    )

    # libmito
    install(
        TARGETS mito
        EXPORT mito-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    # all done
endfunction(mito_mitoLib)

# build the mito extension modules
function(mito_mitoModule)
    # the mito bindings
    Python_add_library(mitomodule MODULE)

    # adjust the name to match what python expects
    set_target_properties(mitomodule PROPERTIES LIBRARY_OUTPUT_NAME mito)
    set_target_properties(mitomodule PROPERTIES SUFFIX ${PYTHON3_SUFFIX})

    # specify the directory for the module compilation products
    mito_library_directory(mitomodule extensions)

    # set the libraries to link against
    target_link_libraries(mitomodule PRIVATE mito pybind11::module)

    # add the sources
    target_sources(mitomodule PRIVATE
        extensions/mito/mito.cc
    )

    # install the mito extensions
    install(
        TARGETS mitomodule
        LIBRARY
        DESTINATION ${MITO_DEST_EXTENSIONS}
    )
endfunction(mito_mitoModule)

# generate a unique target name based on the file name
function(mito_target_name targetname filename)
    # split
    get_filename_component(path ${filename} DIRECTORY)
    get_filename_component(base ${filename} NAME_WE)

    # replace path separators with dots
    string(REPLACE "/" "." stem ${path})

    # build the target name and return it
    set(${targetname} "${stem}.${base}" PARENT_SCOPE)

    # all done
endfunction()

# specify the directory for the target compilation products
function(mito_target_directory target directory)
    # set output directory for this target to subdirectory {directory} of the build directory
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}/${directory}
    )

    # all done
endfunction()

# specify the directory for the module
function(mito_library_directory library directory)
    # set output directory for this library to subdirectory {directory} of the build directory
    set_target_properties(${library} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}/${directory}
    )

    # all done
endfunction()

# add definitions to compilation of file
function(mito_add_definitions driverfile)
    # the argument list is the list of definitions
    set(definitions ${ARGN})

    # generate the name of the target
    mito_target(target ${driverfile})

    # for each definition requested
    foreach(definition IN LISTS definitions)
        # apply the definition to the target
        target_compile_definitions(${target} PRIVATE ${definition})
    endforeach()

    # all done
endfunction()

# end of file
