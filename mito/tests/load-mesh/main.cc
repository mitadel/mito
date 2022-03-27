#include "../../base.h"
#include "../../mesh.h"

int
main()
{
    clock_t t;

    //
    t = clock();
    auto mesh = mito::mesh::mesh<2>("rectangle.summit");
    std::cout << "Loaded mesh in " << clock() - t << std::endl;

    t = clock();
    auto element_set_boundary = mesh.boundary_element_set<1>();
    std::cout << "Fetched boundary in " << clock() - t << std::endl;

    return 0;
}
