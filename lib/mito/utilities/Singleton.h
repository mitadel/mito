// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::utilities {

    template <class RESOURCE_T>
    class Singleton {

        using resource_t = RESOURCE_T;

      public:
        template <class... Args>
        static auto GetInstance(Args &&... args) -> resource_t &
        {
            static resource_t resource(std::forward<Args>(args)...);
            return resource;
        }

        // delete metamethods
        Singleton() = delete;
        ~Singleton() = delete;
        Singleton(const Singleton &) = delete;
        Singleton & operator=(const Singleton &) = delete;
    };

}    // namespace mito


// end of file
