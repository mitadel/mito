// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // TODO: concept for element type
    template <class elementT>
    class Weakform {

      private:
        // the element type
        using element_type = elementT;
        // the number of nodes per element
        static constexpr int n_element_nodes = element_type::n_nodes;
        // the elementary matrix type
        using elementary_matrix_type = tensor::matrix_t<n_element_nodes>;
        // the elementary vector type
        using elementary_vector_type = tensor::vector_t<n_element_nodes>;
        // the type of the lhs assembly block
        using lhs_assembly_block_type =
            blocks::assembly_block_t<element_type, elementary_matrix_type>;
        // a collection of lhs assembly blocks
        using lhs_assembly_blocks_type = std::vector<const lhs_assembly_block_type *>;
        // the type of the rhs assembly block
        using rhs_assembly_block_type =
            blocks::assembly_block_t<element_type, elementary_vector_type>;
        // a collection of rhs assembly blocks
        using rhs_assembly_blocks_type = std::vector<const rhs_assembly_block_type *>;

      public:
        // default constructor
        constexpr Weakform() = default;

        // destructor
        constexpr ~Weakform() = default;

        // delete move constructor
        constexpr Weakform(Weakform &&) noexcept = delete;

        // delete copy constructor
        constexpr Weakform(const Weakform &) = delete;

        // delete assignment operator
        constexpr Weakform & operator=(const Weakform &) = delete;

        // delete move assignment operator
        constexpr Weakform & operator=(Weakform &&) noexcept = delete;

      public:
        // add a left hand side assembly block
        constexpr auto add_block(const lhs_assembly_block_type & block) -> void
        {
            // add the block to the collection
            _lhs_assembly_blocks.push_back(&block);

            // all done
            return;
        }

        // add a right hand side assembly block
        constexpr auto add_block(const rhs_assembly_block_type & block) -> void
        {
            // add the block to the collection
            _rhs_assembly_blocks.push_back(&block);

            // all done
            return;
        }

        // compute the elementary contributions to matrix and right-hand side from the weakform
        constexpr auto compute_blocks(const element_type & element) const
            -> std::pair<elementary_matrix_type, elementary_vector_type>
        {
            // instantiate the elementary matrix
            auto elementary_matrix = elementary_matrix_type();
            // loop on the left hand side assembly blocks
            for (const auto & block : _lhs_assembly_blocks) {
                // compute the elementary contribution of the block
                auto matrix_block = block->compute(element);
                // add the elementary contribution to the elementary matrix
                elementary_matrix += matrix_block;
            }

            // instantiate the elementary vector
            auto elementary_vector = elementary_vector_type();
            // loop on the right hand side assembly blocks
            for (const auto & block : _rhs_assembly_blocks) {
                // compute the elementary contribution of the block
                auto vector_block = block->compute(element);
                // add the elementary contribution to the elementary vector
                elementary_vector += vector_block;
            }

            // return the elementary matrix and vector
            return { elementary_matrix, elementary_vector };
        }

      private:
        // the collection of left hand side assembly blocks
        lhs_assembly_blocks_type _lhs_assembly_blocks;

        // the collection of right hand side assembly blocks
        rhs_assembly_blocks_type _rhs_assembly_blocks;
    };

}    // namespace mito


// end of file
