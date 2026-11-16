// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    template <class semiDiscreteModelT, class solverT, linalg::valid_backend_c linalgBackendT>
    class ImplicitEuler;

    template <class semiDiscreteModelT, class solverT, linalg::valid_backend_c linalgBackendT>
    class CrankNicholson;

    template <class semiDiscreteModelT, class solverT, linalg::valid_backend_c linalgBackendT>
    class SDIRK3S3O;
}


// end of file
