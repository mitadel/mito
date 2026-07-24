// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#pragma once

#include "blocks/InterfaceCouplingBlock.h"

#include <vector>

namespace mito::fem {

    template <class bulkElementT, class interfaceElementT, int n_dof>
    class InterfaceWeakform {

      public:
        using bulk_element_type = bulkElementT;
        using interface_element_type = interfaceElementT;
        using elementary_matrix_type = tensor::matrix_t<n_dof>;
        using lhs_block_type =
            blocks::InterfaceCouplingBlock<bulkElementT, interfaceElementT, elementary_matrix_type>;
        using lhs_block_pointer = const lhs_block_type *;

      public:
        constexpr InterfaceWeakform() = default;

        // register a lhs interface block
        constexpr auto add_block(const lhs_block_type & block) -> void
        {
            _lhs_blocks.push_back(&block);
        }

        constexpr auto compute_blocks(
            const interface_element_type & interface_element,
            const bulk_element_type & bulk_positive, const bulk_element_type & bulk_negative) const
            -> elementary_matrix_type
        {
            auto elementary_matrix = elementary_matrix_type();
            for (const auto * block : _lhs_blocks) {
                elementary_matrix +=
                    block->compute(interface_element, bulk_positive, bulk_negative);
            }
            return elementary_matrix;
        }

      private:
        std::vector<lhs_block_pointer> _lhs_blocks;
    };

}    // namespace mito::fem
