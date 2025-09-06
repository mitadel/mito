// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // TOFIX: the source does not need to be necessarily a scalar field, it can be some other field
    // see if we can use {field_c} instead of {scalar_field_c}
    template <class elementT, class quadratureRuleT, fields::scalar_field_c sourceFieldT>
    class SourceTermBlock : public AssemblyBlock<elementT, tensor::vector_t<elementT::n_nodes>> {

      public:
        // my parent class
        using parent_type = AssemblyBlock<elementT, tensor::vector_t<elementT::n_nodes>>;

        // my template parameters
        using element_type = typename parent_type::element_type;
        using elementary_block_type = typename parent_type::elementary_block_type;
        using quadrature_rule_type = quadratureRuleT;

        // the type of the source term function
        using source_field_type = sourceFieldT;

      public:
        // instantiate the quadrature rule
        static constexpr auto quadrature_rule = quadrature_rule_type();

      public:
        // constructor
        SourceTermBlock(const source_field_type & source_field) : _source_field(source_field) {}

      public:
        // compute the elementary contribution of this block
        auto compute(const element_type & element) const -> elementary_block_type override
        {
            // the number of nodes per element
            constexpr int n_nodes = element_type::n_nodes;

            // the number of quadrature points per element
            constexpr int n_quads = quadrature_rule_type::npoints;

            // the elementary rhs
            elementary_block_type elementary_rhs{};

            // loop on the quadrature points
            tensor::constexpr_for_1<n_quads>([&]<int q>() {
                // the barycentric coordinates of the quadrature point
                constexpr auto xi = quadrature_rule.point(q);

                // the coordinates of the quadrature point
                auto coord = element.parametrization()(xi);

                // precompute the common factor
                auto factor =
                    quadrature_rule.weight(q) * tensor::determinant(element.jacobian()(xi));

                // loop on the nodes of the element
                tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                    // evaluate the a-th shape function at {xi}
                    auto phi_a = element.template shape<a>()(xi);
                    // populate the elementary contribution to the rhs
                    elementary_rhs[{ a }] += factor * _source_field(coord) * phi_a;
                });
            });

            // all done
            return elementary_rhs;
        }

      private:
        // the source term field
        const source_field_type & _source_field;
    };

}    // namespace mito


// end of file
