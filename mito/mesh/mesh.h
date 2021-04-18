#include "../mito.h"
#include "simplex.h"
#include <map>
#include <fstream>

namespace mito {

    template <DIM D>
    class Mesh {

      private:
        // typedef for a collection of mesh entities
        template <typename T>
        using entity_collection = std::vector<T>;

        // typedef for a collection of simplices of dimension I-1
        template <size_t I>
        using simplex_entity_collection = entity_collection<Simplex<I>>;

        // simplex_entity_collection<I>... expands to:
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
        using composition_map = std::map<std::array<Simplex<I - 1> *, I + 1>, Simplex<I> *>;

        template <typename = std::make_index_sequence<D>>
        struct composition_tuple;

        template <size_t... I>
        struct composition_tuple<std::index_sequence<I...>> {
            using type = std::tuple<composition_map<I + 1>...>;
        };

        // this expands to:
        // tuple<composition_map<1>, ..., composition_map<D>
        using composition_tuple_t = typename composition_tuple<>::type;

      public:
        Mesh(std::string meshFileName) : _entities(), _compositions() { _loadMesh(meshFileName); }

        ~Mesh() {}

      public:
        /**
         * @brief Inserts an entity in the composition map
         *
         * @tparam I dimension of the entity to insert
         * @param composition array of the I+1 entities of dimension I-1 composing the entity
         * @param entity mesh entity to be inserted
         * @return the pair returned by the map insertion:
         *          pair::first is an iterator pointing to either the newly inserted element or to
         *                      the element with an equivalent key
         *          pair::second is true (false) if the entity was inserted (was already in the map)
         */
        template <int I>
        auto _insertEntityComposition(
            std::array<Simplex<I - 1> *, I + 1> composition, Simplex<I> & entity)
        {
            // sort the composition container (with the address of the entities) before insertion
            // to guarantee uniqueness with respect to the composing entities
            std::sort(composition.begin(), composition.end());
            return std::get<I - 1>(_compositions)
                .insert(std::pair<std::array<Simplex<I - 1> *, I + 1>, Simplex<I> *>(
                    composition, &entity));
        }

        template <int I>
        void _addEntity(Simplex<I> && entity)
        {
            // TOFIX: is push_back expensive even when we reserve the space?
            std::get<I>(_entities).push_back(entity);

            // all done
            return;
        }

        template <int I>
        auto _getEntity(int n)
        {
            return &std::get<I>(_entities)[n];
        }

        template <int I>
        int _nEntities()
        {
            // all done
            return std::get<I>(_entities).size();
        }

      private:
        void _loadMesh(std::string meshFileName)
        {
            std::cout << "Loading mesh..." << std::endl;

            // open mesh file
            std::ifstream fileStream;
            fileStream.open(meshFileName);

            // read dimension of physical space
            int dim = 0;
            fileStream >> dim;

            // TOFIX
            assert(int(D) == dim);

            // read number of vertices
            int N_vertices = 0;
            fileStream >> N_vertices;
            // reserve space for vertices
            std::get<DIM0>(_entities).reserve(N_vertices);

            // read number of elements
            int N_elements = 0;
            fileStream >> N_elements;
            // reserve space for elements
            std::get<D>(_entities).reserve(N_elements);

            // read number of element sets
            int N_element_sets = 0;
            fileStream >> N_element_sets;

            // QUESTION: Not sure that we need this...
            assert(N_element_sets == 1);

            // fill in vertices
            mito::VertexCoordinatesMap<D> vertexCoordinatesMap;
            for (int n = 0; n < N_vertices; ++n) {
                // instantiate new vertex
                mito::vertex_t * vertex = new mito::vertex_t();
                // instantiate new point
                mito::point_t<D> point;
                for (int d = 0; d < D; ++d) {
                    // read point coordinates
                    fileStream >> point[d];
                }
                // associate the new vertex to the new point
                vertexCoordinatesMap.insert(*vertex, std::move(point));
                _addEntity(std::move(*vertex));
            }
            // vertexCoordinatesMap.print();

            // sanity check: the number of vertices in the map is N_vertices
            assert(_nEntities<DIM0>() == N_vertices);

            for (int i = 0; i < N_elements; ++i) {
                int element_type = 0;
                fileStream >> element_type;

                if (element_type == 3) {

                    int index0 = 0;
                    fileStream >> index0;

                    int index1 = 0;
                    fileStream >> index1;

                    int index2 = 0;
                    fileStream >> index2;

                    mito::vertex_t * vertex0 = _getEntity<DIM0>(index0);
                    mito::vertex_t * vertex1 = _getEntity<DIM0>(index1);
                    mito::vertex_t * vertex2 = _getEntity<DIM0>(index2);

                    // TOFIX: we should avoid the repetition of {vertex0, vertex1} to create the
                    //        segment and to insert it
                    mito::segment_t * segment0 = new mito::segment_t({ *vertex0, *vertex1 });
                    auto ret0 = _insertEntityComposition({ vertex0, vertex1 }, *segment0);
                    mito::segment_t * unique_segment0 = ret0.first->second;
                    // TOFIX: double check this delete... Maybe valgrind the whole thing...
                    if (ret0.second == true) {
                        _addEntity(std::move(*segment0));
                    } else {
                        delete segment0;
                    }

                    mito::segment_t * segment1 = new mito::segment_t({ *vertex1, *vertex2 });
                    auto ret1 = _insertEntityComposition({ vertex1, vertex2 }, *segment1);
                    mito::segment_t * unique_segment1 = ret1.first->second;
                    if (ret1.second == true) {
                        _addEntity(std::move(*segment1));
                    } else {
                        delete segment1;
                    }

                    mito::segment_t * segment2 = new mito::segment_t({ *vertex2, *vertex0 });
                    auto ret2 = _insertEntityComposition({ vertex2, vertex0 }, *segment2);
                    mito::segment_t * unique_segment2 = ret2.first->second;
                    if (ret2.second == true) {
                        _addEntity(std::move(*segment2));
                    } else {
                        delete segment2;
                    }

                    // QUESTION: Can the label be more than one?
                    // read label for element
                    // TOFIX: Ignored for now
                    std::string element_set_id;
                    fileStream >> element_set_id;

                    mito::triangle_t * element = new mito::triangle_t(
                        { *unique_segment0, *unique_segment1, *unique_segment2 });
                    auto ret = _insertEntityComposition(
                        { unique_segment0, unique_segment1, unique_segment2 }, *element);
                    assert(ret.second == true);
                    _addEntity(std::move(*element));
                }
            }

            // sanity check: each element is self-consistent
            for (const auto & element : std::get<D>(_entities)) {
                assert(element.sanityCheck());
            }

            // finalize file stream
            fileStream.close();

            // all done
            return;
        }

      private:
        // container to store D+1 containers of d dimensional entities with d = 0, ..., D
        entities_tuple_t _entities;
        // container to store D maps with the composition of i-dimensional entities in terms
        // of arrays of (i-1)-dimensional entities
        composition_tuple_t _compositions;
    };

}