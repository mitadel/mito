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

    auto cell_0 = topology.triangle({ vertex_0, vertex_1, vertex_3 });
    auto cell_1 = topology.triangle({ vertex_1, vertex_2, vertex_3 });
    topology.triangle({ vertex_2, vertex_4, vertex_3 });
    topology.triangle({ vertex_4, vertex_0, vertex_3 });

    // assert that a segment connecting vertex 0 and 1 exists in the topology  (namely, {segment_0})
    EXPECT_TRUE(topology.exists({ vertex_0, vertex_1 }));
    // assert that a segment connecting vertex 1 and 3 exists in the topology (namely, {segment_1})
    EXPECT_TRUE(topology.exists({ vertex_1, vertex_3 }));
    // assert that a segment connecting vertex 3 and 0 exists in the topology (namely, {segment_2})
    EXPECT_TRUE(topology.exists({ vertex_3, vertex_0 }));
    // assert that a segment connecting vertex 2 and 3 exists in the topology (namely, {segment_4})
    EXPECT_TRUE(topology.exists({ vertex_2, vertex_3 }));

    // assert that the topology now contains 4 simplices
    EXPECT_EQ(topology.n_simplices<2>(), 4);

    // erase the cell with edges {segment_0, segment_1, segment_2}
    topology.erase(cell_0);

    // assert that the topology now contains 3 simplices
    EXPECT_EQ(topology.n_simplices<2>(), 3);

    // assert that a segment connecting vertex 0 and 1 no longer exists in the topology
    // ({segment_0} was erased because it is unused after erasing {cell_0})
    EXPECT_FALSE(topology.exists({ vertex_0, vertex_1 }));
    // assert that a segment connecting vertex 1 and 3 no longer exists in the topology
    // ({segment_1} was erased because it is unused after erasing {cell_0})
    EXPECT_FALSE(topology.exists({ vertex_1, vertex_3 }));
    // assert that a segment connecting vertex 3 and 0 no longer exists in the topology
    // ({segment_2} was also erased because unused after erasing {cell_0})
    EXPECT_FALSE(topology.exists({ vertex_3, vertex_0 }));

    // erase the top cell of the pile (namely, the cell with {segment_0, segment_1, segment_2})
    topology.erase(cell_1);

    // assert that the topology now contains 2 simplices
    EXPECT_EQ(topology.n_simplices<2>(), 2);

    // assert that a segment connecting vertex 1 and 2 no longer exists in the topology
    // ({segment_3} was erased because it is unused after erasing {cell_1})
    EXPECT_FALSE(topology.exists({ vertex_1, vertex_2 }));
    // assert that a segment connecting vertex 3 and 1 no longer exists in the topology
    // ({segment_5} was erased because it is unused after erasing {cell_1})
    EXPECT_FALSE(topology.exists({ vertex_3, vertex_1 }));
    // assert that a segment connecting vertex 2 and 3 no longer exists in the topology
    // ({segment_4} was erased because it is unused after erasing {cell_1})
    EXPECT_FALSE(topology.exists({ vertex_2, vertex_3 }));

    // check that erasing a cell twice does not result in an error
    topology.erase(cell_1);
}