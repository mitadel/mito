#include "../mito.h"
#include "simplex.h"
#include <map>
namespace mito {

    template <DIM D>
    class Mesh {

      private:
        // typedef for a collection of mesh entities
        template <typename T>
        using entity_collection = std::vector<T>;

        // typedef for a collection of simplices of dimension I-1
        template <size_t I>
        using simplex_entity_collection = entity_collection<Simplex<I - 1>>;

        // single_entity_collection<I>... expands to:
        // entity_collection<Simplex<0>>, entity_collection<Simplex<1>>, ...,
        //      entity_collection<Simplex<D>>
        template <typename = std::make_index_sequence<D + 1>>
        struct entities_tuple;

        template <size_t... I>
        struct entities_tuple<std::index_sequence<I...>> {
            using type = std::tuple<simplex_entity_collection<I>...>;
        };

        // this expands to:
        // tuple<entity_collection<Simplex<0>>, entity_collection<Simplex<1>>, ...,
        //      entity_collection<Simplex<D>>
        using entities_tuple_t = typename entities_tuple<>::type;

      private:
        // typedef for a composition map of mesh entities:
        // these maps map:
        //      2 pointers to nodes into a pointer to edge,
        //      3 pointers to edges into a pointer to face, ...
        // std::map<std::array<Simplex<0> *, 2>, Simplex<1> *>  edges composition
        // std::map<std::array<Simplex<1> *, 3>, Simplex<2> *>  faces compositions
        // std::map<std::array<Simplex<2> *, 4>, Simplex<3> *>  volumes compositions
        template <size_t I>
        using composition_map = std::map<std::array<Simplex<I> *, I + 2>, Simplex<I + 1> *>;

        template <typename = std::make_index_sequence<D>>
        struct composition_tuple;

        template <size_t... I>
        struct composition_tuple<std::index_sequence<I...>> {
            using type = std::tuple<composition_map<I>...>;
        };

        // this expands to:
        // tuple<composition_map<0>, composition_map<1>, ..., composition_map<D-1>
        using composition_tuple_t = typename composition_tuple<>::type;

      public:
        Mesh(std::string meshFileName) : _entities(), _compositions() { _loadMesh(meshFileName); }

        ~Mesh() {}


      private:
        void _loadMesh(std::string meshFileName) { return; }

      private:
        // container to store D+1 containers of d dimensional entities with d = 0, ..., D
        entities_tuple_t _entities;
        // container to store D maps with the composition of (i+1)-dimensional entities in terms
        // of arrays of i-dimensional entities
        composition_tuple_t _compositions;
    };

}