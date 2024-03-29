// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_utilities_api_h)
#define mito_utilities_api_h


namespace mito::utilities {

    // shared pointer alias
    template <class resourceT>
    using shared_ptr = SharedPointer<resourceT>;

    // TOFIX: perhaps eventually remove indices?
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
}


#endif

// end of file
