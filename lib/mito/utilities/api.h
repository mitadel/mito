// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::utilities {

    // shared pointer alias
    template <class resourceT>
    using shared_ptr = SharedPointer<resourceT>;

    // wrapper of std shared pointer alias
    template <class resourceT>
    using std_shared_ptr = StdSharedPointer<resourceT>;

    // index type alias
    template <class resourceT>
    using index_t = std::uintptr_t;

    // segmented allocator alias
    template <class resourceT>
    using segmented_allocator_t = SegmentedAllocator<resourceT>;

    // the repository
    template <class sharedPointerT>
    using repository_t = class Repository<sharedPointerT>;

    // segmented vector alias
    template <class resourceT>
    using segmented_vector_t = SegmentedVector<resourceT>;

    // concept of the types having the same dimension
    template <class F1, class F2>
    concept same_dim_c = F1::dim == F2::dim;
}


// end of file
