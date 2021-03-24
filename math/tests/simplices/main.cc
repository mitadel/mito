#include "../../simplex.h"

int main () {

    vertex_t vertex0;
    std::cout << "vertex0 = " << &vertex0 << std::endl;

    vertex_t vertex1;
    std::cout << "vertex1 = " << &vertex1 << std::endl;

    segment_t segment({vertex0, vertex1});
    segment.print();

    // all done
    return 0; 
}
