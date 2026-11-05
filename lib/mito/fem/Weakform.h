// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <class lhsBlockT, class rhsBlockT>
    class Weakform {

      private:
        // the type of the left hand side assembly block
        using lhs_block_type = lhsBlockT;
        // the type of the right hand side assembly block
        using rhs_block_type = rhsBlockT;
        // the elementary matrix type
        template <class elementType>
        using elementary_matrix_type =
            typename lhs_block_type::template elementary_shape_t<elementType>;
        // the elementary vector type
        template <class elementType>
        using elementary_vector_type =
            typename rhs_block_type::template elementary_shape_t<elementType>;

      public:
        // constructor
        constexpr Weakform(const lhsBlockT & lhs_block, const rhsBlockT & rhs_block) :
            _lhs_block(lhs_block),
            _rhs_block(rhs_block)
        {}

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
        // compute the elementary contributions to matrix and right-hand side from the weakform
        template <class elementType>
        constexpr auto compute_lhs_block(const elementType & element) const
            -> elementary_matrix_type<elementType>
        {
            // the elementary matrix
            auto elementary_matrix = _lhs_block.compute(element);

            // return the elementary matrix and vector
            return elementary_matrix;
        }

        // compute the elementary contributions to matrix and right-hand side from the weakform
        template <class elementType>
        constexpr auto compute_rhs_block(const elementType & element) const
            -> elementary_vector_type<elementType>
        {
            // the elementary vector
            auto elementary_vector = _rhs_block.compute(element);

            // return the elementary matrix and vector
            return elementary_vector;
        }

        // compute the elementary contributions to matrix and right-hand side from the weakform
        template <class elementType>
        constexpr auto compute_blocks(const elementType & element) const
            -> std::pair<elementary_matrix_type<elementType>, elementary_vector_type<elementType>>
        {
            // return the elementary matrix and vector
            return { compute_lhs_block(), compute_rhs_block() };
        }


      private:
        // the left hand side assembly block
        lhs_block_type _lhs_block;

        // the right hand side assembly block
        rhs_block_type _rhs_block;
    };

}    // namespace mito


// end of file
