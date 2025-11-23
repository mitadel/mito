// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::discrete {

    template <class keyT, class valueT>
    class DiscreteField {

      private:
        // the key type
        using key_type = keyT;
        // the value type
        using value_type = valueT;

        // a map from {key_type} to {value_type} values
        using map_type =
            std::unordered_map<key_type, value_type, utilities::hash_function<key_type>>;

      public:
        // constructor
        DiscreteField(std::string name) : _map_entry_to_values(), _name(name) {}

        // constructor from a collection of keys
        template <class keysCollectionT>
        DiscreteField(const keysCollectionT & keys, std::string name) : DiscreteField(name)
        {
            // populate the map with the keys and default values for entries
            for (const auto & key : keys) {
                insert(key);
            }
        }

        // destructor
        ~DiscreteField() = default;

        // delete copy constructor
        DiscreteField(const DiscreteField &) = delete;

        // default move constructor
        DiscreteField(DiscreteField &&) = default;

        // delete copy assignment
        auto operator=(const DiscreteField &) -> DiscreteField & = delete;

        // delete move assignment
        auto operator=(DiscreteField &&) -> DiscreteField & = delete;

      public:
        /**
         * accessor for the value of a given entry
         */
        inline auto operator()(const key_type & key) const -> const value_type &
        {
            return _map_entry_to_values.at(key);
        }

        /**
         * mutator for the value of a given entry
         */
        inline auto operator()(const key_type & key) -> value_type &
        {
            return _map_entry_to_values.at(key);
        }

        /**
         * insert a new entry to the field
         */
        inline auto insert(const key_type & key, const value_type & entry = value_type()) -> void
        {
            // add a new entry to the field
            _map_entry_to_values[key] = entry;

            // all done
            return;
        }

        /**
         * accessor for the number of entries
         */
        inline auto size() const -> int { return _map_entry_to_values.size(); }

        /**
         * accessor for name
         */
        inline const std::string & name() const noexcept { return _name; }

        // support for ranged for loops
        inline auto begin() const { return std::cbegin(_map_entry_to_values); }
        inline auto end() const { return std::cend(_map_entry_to_values); }
        inline auto begin() { return std::begin(_map_entry_to_values); }
        inline auto end() { return std::end(_map_entry_to_values); }
        inline auto cbegin() const { return std::cbegin(_map_entry_to_values); }
        inline auto cend() const { return std::cend(_map_entry_to_values); }

      private:
        // the underlying mapping of entries to nodal values
        map_type _map_entry_to_values;

        // the name of the field
        std::string _name;
    };

}    // namespace mito


// end of file
