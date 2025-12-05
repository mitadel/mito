// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::utilities {

    // hash function for shared pointers
    // Note that two pointers pointing to the same cell collapse on the same hashed value
    template <class sharedPointerT>
    struct hash_function {
        size_t operator()(const sharedPointerT & item) const
        {
            // reinterpret the address of the pointed handle as a {size_t} and return it
            return reinterpret_cast<size_t>(item.id());
        }
    };

    // strip constant/volatile and reference from type
    template <typename T>
    using base_type = typename std::remove_cvref_t<T>;

    // function returning the type name of {T} as a string
    template <typename T>
    std::string type_name()
    {
        std::string result;

        // Add qualifiers manually
        if (std::is_const_v<T>)
            result += "const ";
        if (std::is_volatile_v<T>)
            result += "volatile ";
        if (std::is_lvalue_reference_v<T>)
            result += "& ";
        if (std::is_rvalue_reference_v<T>)
            result += "&& ";

        using Base = std::remove_cv_t<std::remove_reference_t<T>>;

        int status = 0;
        char * dem = abi::__cxa_demangle(typeid(Base).name(), nullptr, nullptr, &status);

        if (status == 0) {
            result += dem;
        } else {
            result += typeid(Base).name();    // fallback
        }
        std::free(dem);

        return result;
    }
}


// end of file
