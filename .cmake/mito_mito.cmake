# -*- cmake -*-

# build the mito libraries
function(mito_mitoLib)
    # build the libmito version file
    configure_file(
        mito/version.h.in mito/version.h
        @ONLY
    )
    configure_file(
        mito/version.cc.in mito/version.cc
        @ONLY
    )

    # copy the mito headers over to the staging area
    file(GLOB_RECURSE files
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/mito
        CONFIGURE_DEPENDS
        *.h *.icc
    )

    foreach(file ${files})
        configure_file(mito/${file} mito/${file} COPYONLY)
    endforeach()

    # the libmito target
    add_library(mito SHARED)

    # set the include directories
    target_include_directories(
        mito PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${MITO_DEST_INCLUDE}>
    )

    # add the sources
    target_sources(mito
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/mito/version.cc
    )

    # and the link dependencies
    target_link_libraries(
        mito
        pyre::pyre
        pyre::journal
    )

    # request c++20
    set_property(TARGET mito PROPERTY CXX_STANDARD 20)

    # install all the mito headers
    install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mito
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
    # set the libraries to link against
    target_link_libraries(mitomodule PRIVATE mito pybind11::module)
    # add the sources
    target_sources(mitomodule PRIVATE
        mito/mito.cc
    )

    # install the mito extensions
    install(
        TARGETS mitomodule
        LIBRARY
        DESTINATION ${MITO_DEST_EXTENSIONS}
    )
endfunction(mito_mitoModule)
# end of file
