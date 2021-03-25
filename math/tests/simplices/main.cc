#include "../../simplex.h"

int main () {

    vertex_t vertex0;
    std::cout << "vertex0 = " << vertex0 << std::endl;

    vertex_t vertex1;
    std::cout << "vertex1 = " << vertex1 << std::endl;

    vertex_t vertex2;
    std::cout << "vertex2 = " << vertex2 << std::endl;

    segment_t segmentA({vertex0, vertex1});
    std::cout << "segment = " << segmentA << std::endl;

    segment_t segmentB({vertex1, vertex2});
    std::cout << "segment = " << segmentB << std::endl;

    segment_t segmentC({vertex2, vertex0});
    std::cout << "segment = " << segmentB << std::endl;

    triangle_t triangle({segmentA, segmentB, segmentC});
    std::cout << "triangle = " << triangle << std::endl;

    // all done
    return 0; 
}
