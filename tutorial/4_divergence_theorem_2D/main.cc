// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <mito.h>


// strip namespace
using namespace mito;
using namespace mito::fields;
using mito::quadrature::GAUSS;

// the type of coordinates
using coordinates_t = geometry::cartesian::coordinates_t<2>;

// the function extracting the {x_0} component of a 2D vector
constexpr auto x_0 = geometry::cartesian::x_0<2>;
// the function extracting the {x_1} component of a 2D vector
constexpr auto x_1 = geometry::cartesian::x_1<2>;

// the {e_0} unit vector in 2D
constexpr auto e_0 = geometry::cartesian::e_0<2>;
// the {e_1} unit vector in 2D
constexpr auto e_1 = geometry::cartesian::e_1<2>;


int
main()
{
    // make a channel
    journal::info_t channel("tutorial.divergence_theorem");

    // report
    channel << "a mito application" << journal::endl;

    // a vector field
    auto f = x_0 * x_1 * e_0 + x_0 * x_0 * e_1;

    // the divergence of {f}
    auto div = divergence(f);

    // the coordinate system
    auto coord_system = geometry::coordinate_system<coordinates_t>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mesh::mesh<geometry::triangle_t<2>>();

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

    // build nodes
    auto node_0 = geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = geometry::node(coord_system, { 1.0, 1.0 });
    auto node_3 = geometry::node(coord_system, { 0.5, 0.5 });
    auto node_4 = geometry::node(coord_system, { 0.0, 1.0 });

    // populate the mesh
    mesh.insert({ node_0, node_1, node_3 });
    mesh.insert({ node_1, node_2, node_3 });
    mesh.insert({ node_2, node_4, node_3 });
    mesh.insert({ node_4, node_0, node_3 });

    // create the body manifold
    auto bodyManifold = manifolds::manifold(mesh, coord_system);
    // create the body integrator
    auto bodyIntegrator = quadrature::integrator<GAUSS, 2>(bodyManifold);

    // the integral of the divergence on the body
    auto resultBody = bodyIntegrator.integrate(div);

    // report result
    channel << "Result of body integration = " << resultBody << journal::endl;

    // the normal to the boundary
    auto n = functions::function([](const coordinates_t & x) {
        // the left, right, bottom and top normals
        return (x[0] == 0.0) * (-e_0(x)) + (x[0] == 1.0) * e_0(x) + (x[1] == 0.0) * (-e_1(x))
             + (x[1] == 1.0) * e_1(x);
    });

    // the boundary mesh
    auto boundary_mesh = mesh::boundary(mesh);

    // the boundary manifold
    auto boundary_manifold = manifolds::submanifold(boundary_mesh, coord_system, n);

    // the boundary integrator
    auto boundary_integrator = quadrature::integrator<GAUSS, 2>(boundary_manifold);

    // the integral of the original field on the boundary
    auto resultBoundary = boundary_integrator.integrate(f * n);

    // report result
    channel << "Result of boundary integration = " << resultBoundary << journal::endl;
}

// end of file
