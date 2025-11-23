// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::constraints {

    // Dirichlet class alias
    template <class meshT, fields::field_c fieldT>
    using dirichlet_bc_t = Dirichlet<meshT, fieldT>;

    // factory for Dirichlet boundary conditions
    template <class meshT, fields::field_c fieldT>
    constexpr auto dirichlet_bc(const meshT & mesh, const fieldT & function)
        -> dirichlet_bc_t<meshT, fieldT>;
}


// end of file
