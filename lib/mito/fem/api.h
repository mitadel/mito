// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // quadrature field factory
    template <int Q, class Y>
    constexpr auto quadrature_field(int nElements, std::string name = "");

    // nodal field factory
    template <typename T, int D>
    constexpr auto nodal_field(int nodes, std::string name = "");

}


// end of file
