// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
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
            static resource_t resource(args...);
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
