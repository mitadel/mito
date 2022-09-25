// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SharedPointer_h)
#define mito_utilities_SharedPointer_h


// helper functions
namespace {
    template <class Resource, class... Args, size_t... I>
    inline auto _place_instantiate_object(
        Resource * location, std::tuple<Args...> args, std::index_sequence<I...>)
    {
        return new (location) Resource(std::get<I>(args)...);
    }

    template <class... Args>
    inline auto _last_argument(std::tuple<Args...> args)
    {
        auto constexpr Last = sizeof...(Args) - 1;
        return std::get<Last>(args);
    }
}

namespace mito::utilities {

    // declaration
    template <class Resource, bool isConst, bool immortal>
    class SharedPointer {
        // types
      public:
        using resource_t = Resource;
        using handle_t = std::conditional_t<isConst, const Resource *, Resource *>;

        // interface
      public:
        // cast to handle_t
        inline operator handle_t() const;
        // accessor for the number of outstanding references
        inline auto references() const -> int;
        // operator->
        auto operator->() const -> handle_t;

        // meta methods
      public:
        // destructor
        inline ~SharedPointer();
        // regular constructor (all arguments are forwarded to the constructor of {Resource})
        template <class... Args>
        inline SharedPointer(Args &&... args) requires(
            std::is_constructible_v<Resource, Args &&...>);
        // placement-new constructor: the last argument is the memory location to be used in the
        // placement new, all other arguments are forwarded to the constructor of {Resource}
        template <class... Args>
        inline SharedPointer(Args &&... args) requires(
            // require that the Resource is not constructible from the parameter pack
            !(std::is_constructible_v<Resource, Args &&...>) &&
            // and that the trailing parameter is a pointer to {Resource}
            std::is_same_v<decltype(_last_argument(std::forward_as_tuple(args...))), Resource *>);
        // copy constructor
        inline SharedPointer(const SharedPointer &);
        // copy constructor (const from nonconst)
        template <class OtherResource, bool OtherImmortal>
        inline SharedPointer(const SharedPointer<OtherResource, false, OtherImmortal> &) requires(
            std::is_same_v<Resource, OtherResource> && isConst == true);
        // operator=
        inline SharedPointer & operator=(const SharedPointer &);

      private:
        // increment the reference count
        inline auto _acquire() -> int;
        // decrement the reference count
        inline auto _release() -> int;

        // data members
      private:
        int _count;
        handle_t _handle;
    };
}


// get the inline definitions
#define mito_utilities_SharedPointer_icc
#include "SharedPointer.icc"
#undef mito_utilities_SharedPointer_icc


#endif
// end of file
