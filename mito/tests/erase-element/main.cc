#include "../../base.h"
#include "../../mesh.h"

int
main()
{
    // load mesh of the unit square
    auto mesh = mito::mesh::mesh<2>("small-square.summit");

    // show me the points 
    // std::cout << "Point cloud: " << std::endl;
    // std::cout << mesh.vertices() << std::endl;

    // assert you read 4 elements
    assert(mesh.elements<2>().size() == 4);

    // assert the boundary is made of 4 elements
    assert(mesh.boundary_elements<1>().size() == 4);

    // show me the elements 
    // std::cout << "Initial mesh: " << std::endl;
    // for (const auto & simplex : mesh.elements<2>())
    // {
    //     std::cout << *simplex << std::endl;
    // }

    // erase a simplex
    std::cout << "Erasing simplex..." << std::endl;
    for (const auto & simplex : mesh.elements<2>()) {
        // std::cout << *simplex << std::endl;
        mesh.erase(simplex);
        break;
    }

    // std::cout << "After erase: " << std::endl;
    // for (const auto & simplex : mesh.elements<2>()) {
    //     std::cout << *simplex << std::endl;
    // }

    // assert the boundary is now made of 5 elements
    assert(mesh.boundary_elements<1>().size() == 5);

    // show me the boundary elements
    // std::cout << "Boundary: " << std::endl;
    // for (const auto & simplex : mesh.boundary_elements<1>()) {
    //     std::cout << *simplex << std::endl;
    // }

    // all done
    return 0;
}
