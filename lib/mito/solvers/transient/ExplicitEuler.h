// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::transient {

    template <class discreteSystemT, class matrixSolverT, class mathBackendT>
    // TODO: require that the {matrixSolverT} is compatible with the linear system in
    // {discreteSystemT}
    class ExplicitEuler {

      private:
        // the discrete system type
        using discrete_system_type = discreteSystemT;
        // the matrix solver type
        using matrix_solver_type = matrixSolverT;
        // the math backend type
        using math_backend_type = mathBackendT;
        // the matrix type
        using matrix_type = typename math_backend_type::matrix_type;
        // the vector type
        using vector_type = typename math_backend_type::vector_type;
        // the index type
        using index_type = typename math_backend_type::index_type;
        // the scalar type
        using scalar_type = typename math_backend_type::scalar_type;
        // the options type
        using options_type = std::string;

      public:
        // the default constructor
        constexpr ExplicitEuler(discrete_system_type & discrete_system) :
            _discrete_system(discrete_system),
            // _lumped_mass_vector("lumped_mass_vector"),
            // _stiffness_matrix("stiffness_matrix"),
            // _load_vector("load_vector"),
            _matrix_solver(_discrete_system.linear_system())
        {
            // create the matrix solver
            _matrix_solver.create();
        }

        // destroy the matrix solver
        auto destroy() -> void { return _matrix_solver.destroy(); }

        // set matrix solver options
        auto set_options(const options_type & options) -> void
        {
            return _matrix_solver.set_options(options);
        }

        // solve the matrix system
        auto solve() -> void
        {
            // assemble the discrete system
            // _discrete_system.assemble_lumped_mass(_lumped_mass_vector);
            // _discrete_system.assemble_stiffness(_stiffness_matrix);
            // _discrete_system.assemble_load(_load_vector);
            _discrete_system.assemble_stiffness();
            _discrete_system.assemble_load();

            // solve the linear system
            _matrix_solver.solve();

            // have the discrete system read the solution
            _discrete_system.read_solution();

            // all done
            return;
        }

        // print the matrix system
        auto print() const -> void { return _matrix_solver.print(); }

      private:
        // the discrete system
        discrete_system_type & _discrete_system;
        // the underlying matrix solver implementation
        matrix_solver_type _matrix_solver;
        // // the lumped mass vector
        // vector_type _lumped_mass_vector;
        // // the stiffness matrix
        // matrix_type _stiffness_matrix;
        // // the load vector
        // vector_type _load_vector;
    };


}    // namespace mito::fem


// end of file
