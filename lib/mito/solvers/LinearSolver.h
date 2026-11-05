// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers {

    template <class discreteSystemT, class matrixSolverT, linalg::valid_backend_c linalgBackendT>
    // TODO: require that the {matrixSolverT} is compatible with the linear system in
    // {discreteSystemT}
    class LinearSolver {

      private:
        // the discrete system type
        using discrete_system_type = discreteSystemT;
        // the matrix solver type
        using matrix_solver_type = matrixSolverT;
        // the linear algebra backend type
        using linalg_backend_type = linalgBackendT;
        // the matrix type
        using matrix_type = typename linalg_backend_type::matrix_type;
        // the vector type
        using vector_type = typename linalg_backend_type::vector_type;
        // the options type
        using options_type = std::string;

      public:
        // the default constructor
        constexpr LinearSolver(discrete_system_type & discrete_system) :
            _discrete_system(discrete_system),
            _matrix_solver(),
            _lhs_matrix("lhs", discrete_system.n_equations()),
            _rhs_vector("rhs", discrete_system.n_equations()),
            _solution_vector("solution", discrete_system.n_equations())
        {
            ;
        }

        // set matrix solver options
        auto set_options(const options_type & options) -> void
        {
            return _matrix_solver.set_options(options);
        }

        // solve the matrix system
        auto solve() -> void
        {
            // construct the LHS matrix
            _discrete_system.template assemble_lhs_matrix<linalg_backend_type>(_lhs_matrix);
            _lhs_matrix.assemble();

            // construct the RHS vector
            _discrete_system.template assemble_rhs_vector<linalg_backend_type>(_rhs_vector);
            _rhs_vector.assemble();

            // solve the linear system
            _matrix_solver.solve(_lhs_matrix, _rhs_vector, _solution_vector);

            // have the discrete system read the solution into a field
            _discrete_system.read_solution(_solution_vector);

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

        // the left-hand side matrix
        matrix_type _lhs_matrix;
        // the right-hand side vector
        vector_type _rhs_vector;
        // the solution vector
        vector_type _solution_vector;
    };


}    // namespace mito::fem


// end of file
