// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discretization::blocks {

    // TODO: unify vector and matrix block and allow distinction through a template parameter

    // TODO: implement sum and subtraction operators for the blocks (only for blocks that result in
    // the same elementary type)

    template <class quadratureRuleT, class manifoldT>
    class MatrixBlock {

      public:
        // my template parameters
        using manifold_type = manifoldT;
        using quadrature_rule_type = quadratureRuleT;

        // instantiate the quadrature rule
        static constexpr auto quadrature_rule = quadrature_rule_type();

      public:
        // the constructor
        constexpr MatrixBlock(const manifold_type & manifold) : _manifold(manifold) {}

        // destructor
        constexpr ~MatrixBlock() = default;

        // delete move constructor
        constexpr MatrixBlock(MatrixBlock &&) noexcept = delete;

        // delete copy constructor
        constexpr MatrixBlock(const MatrixBlock &) = delete;

        // delete assignment operator
        constexpr MatrixBlock & operator=(const MatrixBlock &) = delete;

        // delete move assignment operator
        constexpr MatrixBlock & operator=(MatrixBlock &&) noexcept = delete;

      public:
        // TODO: this represents a matrix block because here the return type is a matrix. How shall
        // we make it general to a vector? Shall we have two classes: one MatrixBlock and one
        // VectorBlock?

        // TODO: for now this represents a grad-grad block but we should generalize it to represent
        // any block

        // compute the elementary contribution of this block
        template <class elementT>
        auto compute(const elementT & element) const -> tensor::matrix_t<elementT::n_nodes>
        {
            // the number of nodes per element
            constexpr int n_nodes = elementT::n_nodes;

            // the number of quadrature points per element
            constexpr int n_quads = quadrature_rule_type::npoints;

            // a mito matrix type for the elementary matrix
            using matrix_type = tensor::matrix_t<n_nodes>;
            matrix_type elementary_matrix;

            // get the corresponding cell
            const auto & cell = element.geometric_simplex();

            // compute the volume of the cell
            auto volume = _manifold.volume(cell);

            // loop on the quadrature points
            tensor::constexpr_for_1<n_quads>([&]<int q>() {
                // the barycentric coordinates of the quadrature point
                constexpr auto xi = quadrature_rule.point(q);

                // precompute the common factor
                auto factor = quadrature_rule.weight(q) * volume;

                // loop on the nodes of the element
                tensor::constexpr_for_1<n_nodes>([&]<int a>() {
                    // evaluate the spatial gradient of the element's a-th shape function at {xi}
                    auto dphi_a = element.template gradient<a>()(xi);
                    // loop on the nodes of the element
                    tensor::constexpr_for_1<n_nodes>([&]<int b>() {
                        // evaluate the spatial gradient of the element's b-th shape function at
                        // {xi}
                        auto dphi_b = element.template gradient<b>()(xi);
                        // populate the elementary contribution to the matrix
                        elementary_matrix[{ a, b }] += factor * dphi_a * dphi_b;
                    });
                });
            });

            // all done
            return elementary_matrix;
        }

      private:
        // the manifold
        const manifold_type & _manifold;
    };

}    // namespace mito


// end of file
