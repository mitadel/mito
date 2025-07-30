// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers {

    template <class discreteSystemT, class matrixSolverT>
    // TODO: require that the {matrixSolverT} is compatible with the linear system in
    // {discreteSystemT}
    class LinearSolver {

      private:
        // the discrete system type
        using discrete_system_type = discreteSystemT;
        // the matrix solver type
        using matrix_solver_type = matrixSolverT;
        // the options type
        using options_type = std::string;

      public:
        // the default constructor
        constexpr LinearSolver(discrete_system_type & discrete_system) :
            _discrete_system(discrete_system),
            _matrix_solver(_discrete_system.linear_system())
        {}

        // TOFIX: the need to explicitly call {create} just holds for petsc matrix solvers...
        // create the matrix solver
        auto create() -> void { return _matrix_solver.create(_discrete_system.n_equations()); }

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
            _discrete_system.assemble();

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
    };


}    // namespace mito::discretization


// end of file
