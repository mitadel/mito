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
    )

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

# end of file
