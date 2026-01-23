# -*- cmake -*-
#
# Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
#


# pyre support

# find pyre
find_package(pyre REQUIRED)

# link against pyre libraries
target_link_libraries(mito PUBLIC pyre::journal pyre::pyre)


# end of file
