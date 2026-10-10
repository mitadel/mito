// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <functions::function_c functionT>
    class L2NormBlock {

      public:
        // the type of the elementary shape matrix
        using elementary_shape_t = tensor::scalar_t;

        // the type of the function to compute the L2 norm of
        using function_type = functionT;

      public:
        // constructor
        L2NormBlock(const function_type & function) : _function(function) {}

      public:
        // compute the elementary contribution of this block
        template <int doe, class elementT>
        requires function_in_parametric_coordinates_c<functionT, elementT, doe>
        auto compute(const elementT & element) const -> elementary_shape_t
        {
            // the parametric coordinates type
            using parametric_coordinates_type = typename elementT::parametric_coordinates_type;

            // the quadrature rule type
            using quadrature_rule_type = gauss_rule_t<elementT, doe>;

            // the elementary matrix
            return manifolds::cell_integrator<quadrature_rule_type>(element.element())
                .integrate(mito::functions::function([&](const parametric_coordinates_type & xi) {
                    // the elementary contribution at quadrature point {xi}
                    elementary_shape_t norm{};

                    // evaluate the function at the quadrature point
                    auto fx = _function(xi);

                    // assemble the elementary contribution
                    norm = fx * fx;

                    // all done
                    return norm;
                }));
        }

        // compute the elementary contribution of this block
        template <class elementT>
        auto compute(const elementT & element) const -> elementary_shape_t
        {
            return compute<elementT::degree>(element);
        }

      private:
        // the function to compute the L2 norm of
        const function_type & _function;
    };

}    // namespace mito


// end of file
