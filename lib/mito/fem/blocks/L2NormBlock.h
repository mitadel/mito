// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem::blocks {

    template <class finiteElementT, class quadratureRuleT, functions::function_c functionT>
    // require that {functionT} is a function in parametric coordinates
    requires(std::is_same_v<
             typename functionT::input_type, typename quadratureRuleT::quadrature_point_type>)
    class L2NormBlock {

      public:
        // my finite element type
        using element_type = finiteElementT;
        // my quadrature rule
        using quadrature_rule_type = quadratureRuleT;
        // my elementary shape
        using elementary_shape = tensor::scalar_t;

        // the type of the function to compute the L2 norm of
        using function_type = functionT;

      public:
        // constructor
        L2NormBlock(const function_type & function) : _function(function) {}

      public:
        // compute the elementary contribution of this block
        template <class elementT>
        requires element_of_type_c<elementT, element_type>
        auto compute(const elementT & element) const -> elementary_shape
        {
            // the parametric coordinates type
            using parametric_coordinates_type = typename elementT::parametric_coordinates_type;

            // the elementary matrix
            return manifolds::cell_integrator<quadrature_rule_type>(element.element())
                .integrate(mito::functions::function([&](const parametric_coordinates_type & xi) {
                    // the elementary contribution at quadrature point {xi}
                    elementary_shape norm{};

                    // evaluate the function at the quadrature point
                    auto fx = _function(xi);

                    // assemble the elementary contribution
                    norm = fx * fx;

                    // all done
                    return norm;
                }));
        }

      private:
        // the function to compute the L2 norm of
        const function_type & _function;
    };

}    // namespace mito


// end of file
