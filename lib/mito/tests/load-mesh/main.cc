#include "../../base.h"
#include "../../mesh.h"

int
main()
{
    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::mesh::summit<2>(fileStream);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;

    t = clock();
    auto boundary_elements = mesh.boundary_elements<1>();
    std::cout << "Fetched boundary in " << clock() - t << std::endl;

    return 0;
}
