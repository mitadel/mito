// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // type alias for convenient access to the isoparametric simplex type
    template <int degree, geometry::geometric_simplex_c geometricSimplexT>
    using isoparametric_simplex_t = typename isoparametric_simplex<degree, geometricSimplexT>::type;

}


// end of file
