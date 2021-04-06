#include "../mito.h"
#include "simplex.h"

namespace mito {

template<DIM D>
class Mesh {

  private:
    // typedef for a collection of mesh entities
    template <typename T>
    using entity_collection = std::vector<T>;

    // typedef for a collection of simplices of dimension I-1
    template <size_t I>
    using simplex_entity_collection = entity_collection<Simplex<I-1>>;

    // single_entity_collection<I>... expands to:
    // entity_collection<Simplex<0>>, entity_collection<Simplex<1>>, ..., 
    //      entity_collection<Simplex<D>>
    template <typename = std::make_index_sequence<D+1>>
    struct entities_tuple;

    template <size_t... I>
    struct entities_tuple<std::index_sequence<I...>> {
        using type = std::tuple<simplex_entity_collection<I>...>;
    };

    // this expands to: 
    // tuple<entity_collection<Simplex<0>>, entity_collection<Simplex<1>>, ..., 
    //      entity_collection<Simplex<D>>
    using entities_tuple_t = typename entities_tuple<>::type;

  public:
    Mesh() {}
    ~Mesh() {}

    // container to store D+1 containers of d dimensional entities with d = 0, ..., D
    entities_tuple_t _entities;

};

}