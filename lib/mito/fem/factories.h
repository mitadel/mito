// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_fem_factories_h)
#define mito_fem_factories_h


namespace mito::fem {

    // quadrature field factory
    template <int Q, class Y>
    constexpr auto quadrature_field(int nElements, std::string name)
    {
        return quadrature_field_t<Q, Y>(nElements, name);
    }

    // nodal field factory
    template <typename T, int D>
    constexpr auto nodal_field(int nodes, std::string name)
    {
        return nodal_field_t<T, D>(nodes, name);
    }
}


#endif

// end of file
