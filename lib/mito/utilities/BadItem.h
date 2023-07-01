// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_BadItem_h)
#define mito_utilities_BadItem_h


namespace mito::utilities {

    // declaration
    template <class Resource>
    // requires ReferenceCountedObject<Resource>
    class BadItem : public SharedPointer<Resource> {
        // types
      public:
        using shared_ptr_t = SharedPointer<Resource>;
        using resource_t = Resource;
        using handle_t = Resource *;

        // interface
      public:
        // operator->
        auto operator->() const -> handle_t override;

        // // operator*
        // auto operator*() const -> const resource_t & override;

        // meta methods
      public:
        // destructor
        inline ~BadItem();

        // default constructor
        inline BadItem();

        // copy constructor
        inline BadItem(const BadItem<Resource> &);

        // QUESTION: is any of these method allowed for a bad item?
        // move constructor
        inline BadItem(BadItem<Resource> &&) noexcept = delete;

        // assignment operator
        inline BadItem & operator=(const BadItem<Resource> &) = delete;

        // move assignment operator
        inline BadItem & operator=(BadItem<Resource> &&) = delete;

      private:
        // friendship with SegmentedContainer
        friend class utilities::SegmentedContainer<resource_t>;
    };

}


// get the inline definitions
#define mito_utilities_BadItem_icc
#include "BadItem.icc"
#undef mito_utilities_BadItem_icc


#endif
// end of file
