# -*- cmake -*-


# pyre support

# find pyre
find_package(pyre REQUIRED)

# link against pyre libraries
target_link_libraries(mito pyre::journal pyre::pyre)


# end of file
