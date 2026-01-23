// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::constraints {

    // factory for Dirichlet boundary conditions
    template <class meshT, fields::field_c fieldT>
    constexpr auto dirichlet_bc(const meshT & mesh, const fieldT & function)
        -> dirichlet_bc_t<meshT, fieldT>
    {
        return dirichlet_bc_t<meshT, fieldT>(mesh, function);
    }

}


// end of file
