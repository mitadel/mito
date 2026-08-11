// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::quadrature {

    // an integrator on a manifold {manifoldT} with a quadrature rule of type {quadratureT} and
    // degree of exactness {r}
    template <quadrature_formula quadratureT, int r, class manifoldT>
    class Integrator {

      public:
        // publish my template parameters
        using manifold_type = manifoldT;
        // the cell type of the manifold
        using cell_type = typename manifold_type::cell_type;
        // the parametric coordinates type
        using parametric_coordinates_type = typename cell_type::parametric_coordinates_type;
        // the reference cell type
        using reference_cell_type = typename manifold_type::cell_type::reference_simplex_type;
        // assemble to quadrature rule based on quadrature_type, cell_type, and r
        using quadrature_rule_type = quadrature_rule_t<quadratureT, reference_cell_type, r>;

      public:
        // the constructor
        Integrator(const manifold_type & manifold) : _manifold(manifold) {}

        // integrate field {f}
        auto integrate(const fields::scalar_field_c auto & f) const -> tensor::scalar_t
        {
            // initialize the result
            auto result = tensor::scalar_t{ 0.0 };
            // assemble elementary contributions
            for (const auto & cell : _manifold.elements()) {
                // get cell parametrization under the manifold's coordinate system
                const auto phi = cell.parametrization();
                // assemble the elementary contribution
                result += manifolds::cell_integrator<quadrature_rule_type>(cell).integrate(
                    mito::functions::function(
                        [&](const parametric_coordinates_type & xi) { return f(phi(xi)); }));
            }

            // all done
            return result;
        }

      private:
        // the domain of integration
        const manifold_type & _manifold;
    };

}    // namespace  mito


// end of file
