// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // an archetype of a first-order system M du/dt + K u = f
    template <class archetypeT>
    concept parabolic_archetype_c = std::same_as<archetypeT, models::ParabolicPDE>;

    // a linear first-order problem M du/dt + K u = f, usable with the backend {backendT}
    template <class problemT, class backendT>
    concept linear_first_order_problem_c =
        linalg::valid_backend_c<backendT>
        && requires(
            const problemT & problem, typename backendT::matrix_type & A,
            typename backendT::vector_type & v) {
               problem.n_equations();
               problem.template set_pattern<backendT>(A);
               problem.template assemble_mass<backendT>(A);
               problem.template assemble_stiffness<backendT>(A);
               problem.template assemble_load<backendT>(v);
           };
}


// end of file
