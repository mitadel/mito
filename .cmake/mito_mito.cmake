# -*- cmake -*-
#
# michael a.g. aïvázis <michael.aivazis@para-sim.com>
# (c) 1998-2020 all rights reserved
#

# build the mito libraries
function(mito_mitoLib)

  # build the libmito version file
  configure_file(
    version.cc.in mito/version.cc
    @ONLY
    )
  # copy the mito headers over to the staging area
  file(
    COPY .
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/mito
    FILES_MATCHING PATTERN *.h PATTERN *.icc 
    PATTERN ${CMAKE_CURRENT_BINARY_DIR}/mito/version.cc
    )

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
