
#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

using geometry_t = mito::geometry::geometry_t<2>;
using mesh_t = mito::mesh::mesh_t<mito::topology::triangle_t, 2>;
using vertex_t = mito::topology::vertex_t;
using edge_t = mito::topology::segment_t;

auto
build_mesh(geometry_t & geometry, mesh_t & mesh) -> void
{
    /**
     * Mesh with four cells:
        (0,1)           (1,1)
          4               2
          +---------------+
          | .           . |
          |   .       .   |
          |     .   .     |
          |       . 3     |
          |     .   .     |
          |   .       .   |
          | .           . |
          +---------------+
          0               1
        (0,0)           (1,0)
    */

    auto & point_cloud = geometry.point_cloud();
    auto & topology = geometry.topology();

    auto point0 = point_cloud.point({ 0.0, 0.0 });
    auto point1 = point_cloud.point({ 1.0, 0.0 });
    auto point2 = point_cloud.point({ 1.0, 1.0 });
    auto point3 = point_cloud.point({ 0.5, 0.5 });
    auto point4 = point_cloud.point({ 0.0, 1.0 });

    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();
    auto vertex2 = topology.vertex();
    auto vertex3 = topology.vertex();
    auto vertex4 = topology.vertex();

    // print the vertices ids in order of construction
    // std::cout << vertex0.id() << "\t" << vertex1.id() << "\t" << vertex2.id() << "\t"
    //           << vertex3.id() << "\t" << vertex4.id() << std::endl;

    // add nodes to geometry
    geometry.node(vertex0, point0);
    geometry.node(vertex1, point1);
    geometry.node(vertex2, point2);
    geometry.node(vertex3, point3);
    geometry.node(vertex4, point4);

    auto segment0 = topology.segment({ vertex0, vertex1 });
    auto segment1 = topology.segment({ vertex1, vertex3 });
    auto segment2 = topology.segment({ vertex3, vertex0 });
    auto cell0 = topology.triangle({ segment0, segment1, segment2 });

    auto segment3 = topology.segment({ vertex1, vertex2 });
    auto segment4 = topology.segment({ vertex2, vertex3 });
    auto segment5 = topology.segment({ vertex3, vertex1 });
    auto cell1 = topology.triangle({ segment3, segment4, segment5 });

    auto segment6 = topology.segment({ vertex2, vertex4 });
    auto segment7 = topology.segment({ vertex4, vertex3 });
    auto segment8 = topology.segment({ vertex3, vertex2 });
    auto cell2 = topology.triangle({ segment6, segment7, segment8 });

    auto segment9 = topology.segment({ vertex4, vertex0 });
    auto segment10 = topology.segment({ vertex0, vertex3 });
    auto segment11 = topology.segment({ vertex3, vertex4 });
    auto cell3 = topology.triangle({ segment9, segment10, segment11 });

    mesh.insert(cell0);
    mesh.insert(cell1);
    mesh.insert(cell2);
    mesh.insert(cell3);

    return;
}

auto
populate_collection_of_vertices(const mesh_t & mesh) -> std::set<vertex_t>
{
    // a set collecting all the vertices (without repeated entries)
    std::set<vertex_t> vertex_collection;

    // populate the {vertex_collection}
    // loop on cells in {mesh}
    for (const auto & cell : mesh.cells()) {
        // append vertices of current cell to {vertex_collection}
        cell->vertices(vertex_collection);
    }

    // all done
    return vertex_collection;
}

auto
populate_vertices_map(const std::set<vertex_t> & vertex_collection) -> std::map<vertex_t, int>
{
    // a map between a vertex and an integer id
    std::map<vertex_t, int> vertex_ids;

    // the smallest spare vertex id
    int vertex_id = 0;

    // populate the {vertex_ids} map
    for (const auto & vertex : vertex_collection) {
        vertex_ids[vertex] = vertex_id++;
    }

    // all done
    return vertex_ids;
}

auto
populate_collection_of_edges(const mesh_t & mesh) -> std::set<edge_t>
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

auto
populate_adjacency_map(
    const std::map<vertex_t, int> & vertex_ids, const std::set<edge_t> & edge_collection)
    -> std::map<int, std::set<int>>
{
    // the adjacency map
    std::map<int, std::set<int>> adjacency_map;

    // loop on all edges
    // TOFIX: loop only on the unoriented edges. Here we are redundantly filling the {adjacency_map}
    //      twice, once for each of the two edges orientations, because {edge_collection} contains
    //      internal edges with both orientations
    for (const auto & edge : edge_collection) {
        // get the ids of the two vertices
        vertex_t vertex1 = edge->composition()[0]->footprint();
        vertex_t vertex2 = edge->composition()[1]->footprint();

        adjacency_map[vertex_ids.at(vertex1)].insert(vertex_ids.at(vertex2));
        adjacency_map[vertex_ids.at(vertex2)].insert(vertex_ids.at(vertex1));
    }

    // all done
    return adjacency_map;
}

auto
populate_metis_partition(
    int nVertices, int nEdges, int nPartitions, const std::map<int, std::set<int>> & adjacency_map)
    -> void
{
    idx_t nVertices_metis = nVertices;
    idx_t nEdges_metis = nEdges;
    idx_t nWeights = 1;
    idx_t nPartitions_metis = nPartitions;

    idx_t objval;
    std::vector<idx_t> partitions(nVertices_metis, 0);

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
        &nPartitions_metis, NULL, NULL, options, &objval, partitions.data());

    // assert metis ran correctly
    EXPECT_EQ(metisResult, 1);

    std::cout << "partition" << std::endl;
    for (unsigned part_i = 0; part_i < partitions.size(); part_i++) {
        std::cout << part_i << " " << partitions[part_i] << std::endl;
    }

    // all done
    return;
}

auto
partition(const mesh_t & mesh, int nPartitions) -> void
{
    // a collection of all the vertices (without repeated entries)
    auto vertex_collection = populate_collection_of_vertices(mesh);
    // check that you found 5 vertices
    EXPECT_EQ(vertex_collection.size(), 5);

    // a map between the vertices and an integer id
    auto vertex_ids = populate_vertices_map(vertex_collection);
    // assert you assigned 5 labels
    EXPECT_EQ(vertex_ids.size(), 5);

    // a collection of all the edges (without repeated entries)
    auto edge_collection = populate_collection_of_edges(mesh);
    // check that you found 12 edges
    EXPECT_EQ(edge_collection.size(), 12);

    // std::cout << "vertex map" << std::endl;
    // for (const auto & vertex : vertex_ids) {
    //     std::cout << vertex.first << "\t" << vertex.second << std::endl;
    // }

    // the adjacency map
    std::map<int, std::set<int>> adjacency_map =
        populate_adjacency_map(vertex_ids, edge_collection);

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
    int nEdges = mesh.cells().size() * 3 + mesh.boundary_size();
    EXPECT_EQ(nEdges, 16);

    // call metis partitioner
    populate_metis_partition(nVertices, nEdges, nPartitions, adjacency_map);

    // all done
    return;
}

TEST(MetisPartitioner, Base)
{
    // an empty topology
    auto topology = mito::topology::topology();

    // an empty cloud of points
    auto point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    // populate the mesh
    build_mesh(geometry, mesh);

    // number of partitions
    int n_partitions = 2;

    // partition the mesh
    partition(mesh, n_partitions);

    // all done
    return;
}