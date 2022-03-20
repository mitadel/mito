#include "../../base.h"
#include "../../mesh.h"

int
main()
{
    clock_t t;

    //
    t = clock();
    mito::mesh::Mesh<2> mesh("rectangle.summit");
    std::cout << "Loaded mesh (without repeated elements) in " << clock() - t << std::endl;

    return 0;
}
