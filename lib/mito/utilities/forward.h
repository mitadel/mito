// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::utilities {

    // base class for a reference counted object
    class Shareable;

    // concept for a reference counted object
    template <typename resourceT>
    concept reference_countable_c = std::is_base_of<Shareable, resourceT>::value;

    // base class for an invalidatable object
    class Invalidatable;

    // concept for an invalidatable object
    template <typename resourceT>
    concept invalidatable_c = std::is_base_of<Invalidatable, resourceT>::value;

    // class shared pointer based on a reference counted resource
    template <class resourceT>
    // requires reference_countable_c<resourceT>
    class SharedPointer;

    // wrapper class for std shared pointer
    template <class resourceT>
    class StdSharedPointer;

    // class segmented allocator
    template <class resourceT>
    class SegmentedAllocator;

    // and its iterator
    template <class SegmentedAllocatorT>
    class SegmentedAllocatorIterator;

    // class segmented container iterator
    template <class SegmentedContainerT>
    class SegmentedContainerIterator;

    // the repository
    template <class sharedPointerT>
    // requires reference_countable_c<typename sharedPointerT::resource_type>
    class Repository;

    // class segmented vector
    template <class resourceT>
    requires invalidatable_c<resourceT>
    class SegmentedVector;
}


// end of file
