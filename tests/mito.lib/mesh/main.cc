#include <gtest/gtest.h>
#include <mito/mito.h>

TEST(Mesh, TestMeshBuild)
{
    /**
     * Mesh with four elements:
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

    auto topology = mito::topology::topology();

    // TOFIX
    auto vertex0 = topology.vertex(/*mito::geometry::point(0.0, 0.0)*/);
    auto vertex1 = topology.vertex(/*mito::geometry::point(1.0, 0.0)*/);
    auto vertex2 = topology.vertex(/*mito::geometry::point(1.0, 1.0)*/);
    auto vertex3 = topology.vertex(/*mito::geometry::point(0.5, 0.5)*/);
    auto vertex4 = topology.vertex(/*mito::geometry::point(0.0, 1.0)*/);

    auto segment0 = topology.segment({ vertex0, vertex1 });
    auto segment1 = topology.segment({ vertex1, vertex3 });
    auto segment2 = topology.segment({ vertex3, vertex0 });
    auto segment3 = topology.segment({ vertex1, vertex2 });
    auto segment4 = topology.segment({ vertex2, vertex3 });
    auto segment5 = topology.segment({ vertex4, vertex3 });
    auto segment6 = topology.segment({ vertex2, vertex4 });
    auto segment7 = topology.segment({ vertex4, vertex0 });

    auto element0 = topology.triangle({ segment0, segment1, segment2 });
    auto element1 = topology.triangle({ segment3, segment4, segment1 });
    auto element2 = topology.triangle({ segment6, segment5, segment4 });
    auto element3 = topology.triangle({ segment7, segment2, segment5 });

    // TOFIX
    // instantiate mesh of simplicial topology
    // mito::mesh::Mesh<2, mito::topology::simplex_t> mesh;
    // mesh.addSimplex(element0);
    // mesh.addSimplex(element1);
    // mesh.addSimplex(element2);
    // mesh.addSimplex(element3);

    return;
}

TEST(Mesh, TestMeshQuadrature)
{
    // load mesh
    std::ifstream fileStream("square.summit");
    auto mesh = mito::mesh::summit<2>(fileStream);
    // instantiate a element set as a collection of simplices and vertices.
    const auto & elements = mesh.elements<2>();    // TODO: region label to fetch elements
    auto elementSet = mito::manifolds::manifold<2>(elements);

    // instantiate a scalar field
    auto f = mito::math::function([](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); });
    auto f_cosine = mito::math::field(f);

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(elementSet);

    auto result = integrator.integrate(f_cosine);
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - 0.946083) << std::endl;
    EXPECT_NEAR(result, 0.946083, 1.e-7);
}
