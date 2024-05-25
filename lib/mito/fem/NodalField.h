// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    template <int D, class Y>
    class NodalField {

      private:
        // the node type
        using node_type = geometry::node_t<D>;

        // hash function for nodes
        struct hash_function {
            auto operator()(const node_type & node) const -> uintptr_t
            {
                // reinterpret the address of the node as a {uintptr_t} and return it
                return uintptr_t(&node);
            }
        };

        // a map from {key_type} to {Y} values
        using map_type = std::unordered_map<node_type, Y, hash_function>;

      public:
        // constructor
        NodalField(const mesh::mesh_c auto & mesh, std::string name) :
            _map_nodes_to_values(),
            _name(name)
        {
            // populate the map with the mesh nodes (map all nodes to default item {Y})
            for (const auto & cell : mesh.cells()) {
                for (const auto & node : cell.nodes()) {
                    _map_nodes_to_values[node] = Y();
                }
            }
        }

        // destructor
        ~NodalField() = default;

      public:
        /**
         * Operator()
         */
        inline auto operator()(const node_type & node) const -> const Y &
        {
            return _map_nodes_to_values[node];
        }

        inline auto operator()(const node_type & node) -> Y & { return _map_nodes_to_values[node]; }

        /**
         * accessor for the number of nodes
         */
        inline auto n_nodes() const -> int { return _map_nodes_to_values.size(); }

        /**
         * accessor for name
         */
        inline const std::string & name() const noexcept { return _name; }

        // support for ranged for loops (wrapping grid)
        inline auto begin() const { return std::cbegin(_map_nodes_to_values); }
        inline auto end() const { return std::cend(_map_nodes_to_values); }
        inline auto begin() { return std::begin(_map_nodes_to_values); }
        inline auto end() { return std::end(_map_nodes_to_values); }

      private:
        // the underlying mapping of nodes to nodal values
        map_type _map_nodes_to_values;

        // the name of the field
        std::string _name;
    };

    template <int D, class Y>
    std::ostream & operator<<(std::ostream & os, const NodalField<D, Y> & nodalField)
    {

        os << "Nodal field \"" << nodalField.name() << "\" : " << std::endl;


        for (auto i = 0; i < nodalField.n_nodes(); ++i) {
            os << "Node " << i << ": " << nodalField(i) << std::endl;
        }

        return os;
    }

}    // namespace mito


// end of file
