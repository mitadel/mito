// code guard
#if !defined(mito_mesh_metis_partitioner_h)
#define mito_mesh_metis_partitioner_h

// helper functions
namespace {

    template <class meshT>
    auto populate_collection_of_vertices(const meshT & mesh) -> std::vector<vertex_t>
    {
        // a set collecting all the vertices (without repeated entries)
        std::set<vertex_t> vertex_collection;

        // populate the {vertex_collection}
        // loop on cells in {mesh}
        for (const auto & cell : mesh.cells()) {
            // append vertices of current cell to {vertex_collection}
            cell->vertices(vertex_collection);
        }

        // a vector with all vertices
        std::vector<vertex_t> id_to_vertex(vertex_collection.begin(), vertex_collection.end());

        // all done
        return id_to_vertex;
    }

    auto populate_vertices_map(const std::vector<vertex_t> & id_to_vertex)
        -> std::map<vertex_t, int>
    {
        // a map between a vertex and an integer id
        std::map<vertex_t, int> vertex_to_id;

        // populate the {vertex_to_id} map
        for (size_t id = 0; id < id_to_vertex.size(); ++id) {
            vertex_to_id[id_to_vertex[id]] = id;
        }

        // all done
        return vertex_to_id;
    }

    template <class meshT>
    auto populate_collection_of_edges(const meshT & mesh) -> std::set<edge_t>
    {
        // a set collecting all the edges (without repeated entries)
        std::set<edge_t> edge_collection;

        // populate the {edge_collection}
        // loop on cells in {mesh}
        for (const auto & cell : mesh.cells()) {
            // append edges of current cell to {edge_collection}
            cell->edges(edge_collection);
        }

        // all done
        return edge_collection;
    }

    auto populate_adjacency_map(
        const std::map<vertex_t, int> & vertex_to_id, const std::set<edge_t> & edge_collection)
        -> std::map<int, std::set<int>>
    {
        // the adjacency map
        std::map<int, std::set<int>> adjacency_map;

        // loop on all edges
        // TOFIX: loop only on the unoriented edges. Here we are redundantly filling the
        // {adjacency_map}
        //      twice, once for each of the two edges orientations, because {edge_collection}
        //      contains internal edges with both orientations
        for (const auto & edge : edge_collection) {
            // get the ids of the two vertices
            vertex_t vertex1 = edge->composition()[0]->footprint();
            vertex_t vertex2 = edge->composition()[1]->footprint();

            adjacency_map[vertex_to_id.at(vertex1)].insert(vertex_to_id.at(vertex2));
            adjacency_map[vertex_to_id.at(vertex2)].insert(vertex_to_id.at(vertex1));
        }

        // all done
        return adjacency_map;
    }

    auto metis_paint_partition(
        int nVertices, int nEdges, int nPartitions,
        const std::map<int, std::set<int>> & adjacency_map) -> std::vector<idx_t>
    {
        idx_t nVertices_metis = nVertices;
        idx_t nEdges_metis = nEdges;
        idx_t nWeights = 1;
        idx_t nPartitions_metis = nPartitions;

        idx_t objval;
        std::vector<idx_t> painting(nVertices_metis, 0);

        // Indexes of starting points in adjacent array
        std::vector<idx_t> xadjacency(nVertices_metis + 1);
        xadjacency[0] = 0;

        // Adjacent vertices in consecutive index order
        std::vector<idx_t> adjacency(nEdges_metis);

        // populate the {} map
        for (const auto & vertex : adjacency_map) {
            auto vertex_id = vertex.first;
            auto vertex_adj = vertex.second;
            xadjacency[vertex_id + 1] = xadjacency[vertex_id] + vertex_adj.size();

            int count = 0;
            for (auto i : vertex_adj) {
                adjacency[xadjacency[vertex_id] + count] = i;
                count++;
            }
        }

        // std::cout << "xadjacency" << std::endl;
        // for (size_t i = 0; i < xadjacency.size(); ++i) {
        //     std::cout << xadjacency[i] << std::endl;
        // }

        // std::cout << "adjacency" << std::endl;
        // for (size_t i = 0; i < adjacency.size(); ++i) {
        //     std::cout << adjacency[i] << std::endl;
        // }

        // Define the options for METIS
        idx_t options[METIS_NOPTIONS];
        METIS_SetDefaultOptions(options);

        int metisResult = METIS_PartGraphKway(
            &nVertices_metis, &nWeights, xadjacency.data(), adjacency.data(), NULL, NULL, NULL,
            &nPartitions_metis, NULL, NULL, options, &objval, painting.data());

        // all done
        return painting;
    }

}

namespace mito::mesh {

    template <class meshT>
    auto partition(const meshT & mesh, int nPartitions) -> void
    {
        // a collection of all the vertices
        auto id_to_vertex = populate_collection_of_vertices(mesh);

        // a map between the vertices and an integer id
        auto vertex_to_id = populate_vertices_map(id_to_vertex);

        // a collection of all the edges (without repeated entries)
        auto edge_collection = populate_collection_of_edges(mesh);

        // std::cout << "vertex map" << std::endl;
        // for (const auto & vertex : vertex_to_id) {
        //     std::cout << vertex.first << "\t" << vertex.second << std::endl;
        // }

        // the adjacency map
        std::map<int, std::set<int>> adjacency_map =
            populate_adjacency_map(vertex_to_id, edge_collection);

        // for (const auto & vertex : adjacency_map) {
        //     std::cout << vertex.first << std::endl;

        //     for (auto i : vertex.second) {
        //         std::cout << i << " ";
        //     }

        //     std::cout << std::endl;
        //     std::cout << std::endl;
        // }

        int nVertices = mesh.geometry().point_cloud().size();

        // compute the total number of edges (with repeated orientation)
        // TOFIX: is this general?
        int nEdges = mesh.cells().size() * 3 + mesh.boundary_size();

        // call metis partitioner
        auto painting = metis_paint_partition(nVertices, nEdges, nPartitions, adjacency_map);

        std::cout << "partition" << std::endl;
        for (size_t part_i = 0; part_i < painting.size(); part_i++) {
            std::cout << part_i << " " << painting[part_i] << std::endl;
        }

        // all done
        return;
    }
}


#endif

// end of file
