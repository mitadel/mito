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

    // segmented container alias
    template <class resourceT>
    using segmented_t = SegmentedContainer<resourceT>;

    // the repository
    template <class sharedResourceT>
    using repository_t = class Repository<sharedResourceT>;
}


#endif

// end of file
