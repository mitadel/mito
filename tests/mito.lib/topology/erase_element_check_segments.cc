#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/topology.h>


TEST(Topology, EraseElement)
{
    /**
     * Mesh with four cells:
    (0,1)           (1,1)
      4       6       2
      +---------------+
      | .     2     . |
      | 11.7     8.4  |
      |     .   .     |
    9 |  3    +3   1  | 3
      |     .   .     |
      | 10.2     1.5  |
      | .     0     . |
      +---------------+
      0       0       1
    (0,0)           (1,0)
    */

    // an empty topology
    auto & topology = mito::topology::topology();

    auto vertex_0 = topology.vertex();
    auto vertex_1 = topology.vertex();
    auto vertex_2 = topology.vertex();
    auto vertex_3 = topology.vertex();
    auto vertex_4 = topology.vertex();

    auto segment_0 = topology.segment({ vertex_0, vertex_1 });
    auto segment_1 = topology.segment({ vertex_1, vertex_3 });
    auto segment_2 = topology.segment({ vertex_3, vertex_0 });
    auto cell_0 = topology.triangle({ segment_0, segment_1, segment_2 });

    auto segment_3 = topology.segment({ vertex_1, vertex_2 });
    auto segment_4 = topology.segment({ vertex_2, vertex_3 });
    auto segment_5 = topology.segment({ vertex_3, vertex_1 });
    auto cell_1 = topology.triangle({ segment_3, segment_4, segment_5 });

    // assert that the topology now contains 2 simplices
    EXPECT_EQ(topology.n_simplices<2>(), 2);

    // assert that a cell with the composition of {cell_0} does exist within the topology
    EXPECT_TRUE(topology.exists<2>({ segment_0, segment_1, segment_2 }));

    // assert that the cell with the composition of {cell_1} does exist within the topology
    EXPECT_TRUE(topology.exists<2>({ segment_3, segment_4, segment_5 }));

    // erase the cell with edges {segment_0, segment_1, segment_2}
    topology.erase(cell_0);

    // assert that the topology now contains 1 simplex
    EXPECT_EQ(topology.n_simplices<2>(), 1);

    // assert that a cell with the composition of {cell_0} does not exist any more
    EXPECT_FALSE(topology.exists<2>({ segment_0, segment_1, segment_2 }));

    // erase the top cell of the pile (namely, the cell with {segment_0, segment_1, segment_2})
    topology.erase(cell_1);

    // assert that the topology now contains 0 simplices
    EXPECT_EQ(topology.n_simplices<2>(), 0);

    // assert that the cell with the composition of {cell_1} does not exist any more
    EXPECT_FALSE(topology.exists<2>({ segment_3, segment_4, segment_5 }));

    // check that erasing a cell twice does not result in an error
    topology.erase(cell_1);
}