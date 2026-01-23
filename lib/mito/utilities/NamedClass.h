// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


// code guard
#pragma once


namespace mito::utilities {

    // TODO: interface with {libuuid} to generate unique identifiers instead of "unnamed"
    //
    // Class {NamedClass} is meant to attach a string name identifier to a given class
    // {classToName}. After implementing class {classToName}, you can define the alias
    // {class_to_name_t} as follows:
    //       using class_to_name_t = NamedClass<classToName>
    // You can then create named instances of the class by using any of the {classToName}
    // constructors (in which case, a default name will be used for the class) or by prepending the
    // chosen instance name before the other constructor input arguments.
    //
    template <class classToNameT>
    class NamedClass : public classToNameT {
        // types
      public:
        // my template parameter
        using class_to_name_type = classToNameT;
        // me
        using named_class_type = NamedClass<class_to_name_type>;
        // string identifier type
        using name_type = std::string;

        // meta methods
      public:
        // default constructor
        template <class... Args>
        requires std::constructible_from<class_to_name_type, Args...>
        constexpr NamedClass(Args &&... args) :
            class_to_name_type(std::forward<Args>(args)...),
            _name("unnamed")
        {}

        // named constructor
        template <class... Args>
        requires std::constructible_from<class_to_name_type, Args...>
        constexpr NamedClass(const name_type & name, Args &&... args) :
            class_to_name_type(std::forward<Args>(args)...),
            _name(name)
        {}

        // destructor
        constexpr ~NamedClass() = default;

        // copy constructor
        constexpr NamedClass(const named_class_type &) = delete;

        // move constructor
        constexpr NamedClass(named_class_type &&) = delete;

      public:
        // accessor to name
        constexpr auto name() const -> name_type { return _name; }

      private:
        // string identifier
        name_type _name;
    };

}


// end of file
