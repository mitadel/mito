// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization {

    // TOFIX: create a constructor that takes no constraints

    // TOFIX: {constraints} should be a collection of constraints as opposed to an instance of a
    // single constraint
    // function space factory
    template <int p, manifolds::manifold_c manifoldT, constraints::constraint_c constraintsT>
    constexpr auto function_space(const manifoldT & manifold, const constraintsT & constraints)
    {
        // the manifold type
        using manifold_type = manifoldT;
        // the mesh type
        using mesh_type = typename manifold_type::mesh_type;
        // the cell type
        using cell_type = typename mesh_type::cell_type;
        // the degree of the finite element
        constexpr int degree = p;
        // typedef for a finite element
        using element_type = typename isoparametric_simplex<degree, cell_type>::type;

        // build a function space on the manifold and return it
        return function_space_t<element_type, constraintsT>(manifold, constraints);
    }

}


// end of file
