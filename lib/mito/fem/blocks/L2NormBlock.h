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
        // my template parameters
        using element_type = finiteElementT;
        using elementary_block_type = tensor::scalar_t;
        using quadrature_rule_type = quadratureRuleT;

        // the type of the function to compute the L2 norm of
        using function_type = functionT;

      public:
        // instantiate the quadrature rule
        static constexpr auto quadrature_rule = quadrature_rule_type();

      public:
        // constructor
        L2NormBlock(const function_type & function) : _function(function) {}

      public:
        // compute the elementary contribution of this block
        template <class elementT>
        requires element_of_type_c<elementT, element_type>
        auto compute(const elementT & element) const -> elementary_block_type
        {
            // the number of quadrature points per element
            constexpr int n_quads = quadrature_rule_type::npoints;

            // the elementary contribution to the L2 norm
            elementary_block_type norm{};

            // loop on the quadrature points
            tensor::constexpr_for_1<n_quads>([&]<int q>() {
                // the parametric coordinates of the quadrature point
                constexpr auto xi = quadrature_rule.point(q);

                // the measure of the canonical simplex
                constexpr auto measure =
                    element_type::mesh_cell_type::reference_simplex_type::measure;

                // the quadrature weight at this point scaled with the area of the canonical simplex
                constexpr auto w = measure * quadrature_rule.weight(q);

                // precompute the common factor
                auto factor = w * element.volume_element()(xi);

                // populate the elementary contribution to the matrix
                norm += factor * _function(xi) * _function(xi);
            });

            // all done
            return norm;
        }

      private:
        // the function to compute the L2 norm of
        const function_type & _function;
    };

}    // namespace mito


// end of file
