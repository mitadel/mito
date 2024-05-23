// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // quadrature field factory
    template <int Q, class Y>
    constexpr auto quadrature_field(int nElements, std::string name)
    {
        return quadrature_field_t<Q, Y>(nElements, name);
    }

    // nodal field factory
    template <class Y>
    constexpr auto nodal_field(int nodes, std::string name)
    {
        return nodal_field_t<Y>(nodes, name);
    }
}


// end of file
