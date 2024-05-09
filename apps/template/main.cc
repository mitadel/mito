// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// mito support
#include <mito.h>


int
main()
{
    // make a channel
    pyre::journal::info_t channel("apps.template");

    // report
    channel << "the mito application template" << pyre::journal::endl;

    // all done
    return 0;
}