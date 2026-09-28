// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // a term of a coupled weakform: a block to be assembled over the elements of {source}
    template <class sourceT, class blockT>
    struct Term {
        // my template parameters
        using source_type = sourceT;
        using block_type = blockT;

        // the elements this term is assembled over
        const source_type & source;
        // the block computed on each of them
        block_type block;
    };

    // Class {CoupledWeakform} is one weak form as a sum of terms, each on its own domain:
    // a(u, v) = sum_k a_k(u, v). Terms on different function spaces are what couples them
    template <class... termTs>
    class CoupledWeakform {

      public:
        // the type of the collection of terms
        using terms_type = std::tuple<termTs...>;

      public:
        // the constructor
        constexpr CoupledWeakform(const termTs &... terms) : _terms(terms...) {}

      public:
        // accessor for the terms of the weakform
        constexpr auto terms() const noexcept -> const terms_type & { return _terms; }

      private:
        // the terms of the weakform
        terms_type _terms;
    };

}    // namespace mito


// end of file
