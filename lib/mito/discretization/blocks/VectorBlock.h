// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    template <class quadratureRuleT>
    class VectorBlock {

      public:
        // my template parameters
        using quadrature_rule_type = quadratureRuleT;

        // instantiate the quadrature rule
        static constexpr auto quadrature_rule = quadrature_rule_type();

      public:
        // the constructor
        constexpr VectorBlock() {}

        // destructor
        constexpr ~VectorBlock() = default;

        // delete move constructor
        constexpr VectorBlock(VectorBlock &&) noexcept = delete;

        // delete copy constructor
        constexpr VectorBlock(const VectorBlock &) = delete;

        // delete assignment operator
        constexpr VectorBlock & operator=(const VectorBlock &) = delete;

        // delete move assignment operator
        constexpr VectorBlock & operator=(VectorBlock &&) noexcept = delete;

      public:
        // TOFIX: fix {f} dependency
        // compute the elementary contribution of this block
        template <class elementT>
        auto compute(const elementT & element, const auto & function) const
            -> tensor::vector_t<elementT::n_nodes>
        {
            // the number of nodes per element
            constexpr int n_nodes = elementT::n_nodes;

            // the number of quadrature points per element
            constexpr int n_quads = quadrature_rule_type::npoints;

            // a mito vector type for the elementary rhs
            using vector_type = tensor::vector_t<n_nodes>;
            vector_type elementary_rhs;

            // loop on the quadrature points
            tensor::constexpr_for_1<n_quads>([&]<int q>() {
                // the barycentric coordinates of the quadrature point
                constexpr auto xi = quadrature_rule.point(q);

                // the coordinates of the quadrature point
                auto coord = element.parametrization()(quadrature_rule.point(q));

                // precompute the common factor
                auto factor =
                    quadrature_rule.weight(q) * mito::tensor::determinant(element.jacobian()(xi));

                // loop on the nodes of the element
                tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                    // evaluate the a-th shape function at {xi}
                    auto phi_a = element.template shape<a>()(xi);
                    // populate the elementary contribution to the rhs
                    elementary_rhs[{ a }] += factor * function(coord) * phi_a;
                });
            });

            // all done
            return elementary_rhs;
        }
    };

}    // namespace mito


// end of file
