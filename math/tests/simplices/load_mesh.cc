#include "../../simplex.h"
#include "../../../mito.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

template<typename T>
void readUntilNextSpace(std::istream * const stream, T & word) {

    // tools for reading file
    std::string line;
    std::stringstream sstream;

    // get line until separator
    std::getline(*stream, line, ' ');

    // convert the string into the type to return
    sstream.clear();
    sstream << line;
    sstream >> word; 

    // all done
    return;
}

void ignoreRestOfLine(std::istream * const stream) {
    // get rest of the line
    std::string line;
    std::getline(*stream, line);
    // all done
    return;
}

template <mito::DIM D>
bool LoadMesh(std::string fileName) {

    // open mesh file
    std::ifstream fileStream;
    fileStream.open(fileName);

    // read dimension of physical space
    int dim = 0;
    readUntilNextSpace(&fileStream, dim);

    // TOFIX
    assert(int(D) == dim);

    // read number of vertices
    int N_vertices = 0; 
    readUntilNextSpace(&fileStream, N_vertices);

    // read number of elements
    int N_elements = 0; 
    readUntilNextSpace(&fileStream, N_elements);

    // read number of element sets
    int N_element_sets = 0; 
    readUntilNextSpace(&fileStream, N_element_sets);
 
    // TODO: Subdivide by materials?
    assert(N_element_sets == 1);

    // fill in vertices
    std::vector<mito::vertex_t*> vertices(N_vertices);
    mito::VertexCoordinatesMap<D> vertexCoordinatesMap; 
    for (auto & vertex : vertices) {
        // instantiate new vertex
        vertex = new mito::vertex_t();
        // instantiate new point
        mito::point_t<D> * point = new mito::point_t<D>();
        for (int d = 0; d < D; ++d) {
            // read vertices coordinates
            readUntilNextSpace(&fileStream, (*point)[d]);
        }
        // associate the new vertex with the new point
        vertexCoordinatesMap.insert(*vertex, std::move(*point));
    }

    // sanity check
    assert(vertexCoordinatesMap.size() == N_vertices);
    //vertexCoordinatesMap.print();

    // TOFIX: How to properly resize elements? Should this be a vector of pointers to elements?
    //std::vector<mito::Simplex<D>> elements(N_elements);
    std::vector<mito::Simplex<D>> elements;

    for (int i = 0; i < N_elements; ++i) {
        int dim_element = 0; 
        readUntilNextSpace(&fileStream, dim_element);

        if (dim_element == 3) {

            int index0 = 0; 
            readUntilNextSpace(&fileStream, index0);

            int index1 = 0; 
            readUntilNextSpace(&fileStream, index1);

            int index2 = 0; 
            readUntilNextSpace(&fileStream, index2);

            mito::segment_t segment0({*vertices[index0], *vertices[index1]});
            mito::segment_t segment1({*vertices[index1], *vertices[index2]});
            mito::segment_t segment2({*vertices[index2], *vertices[index0]});

            mito::triangle_t element ({segment0, segment1, segment2});

            int element_set_id = 0; 
            readUntilNextSpace(&fileStream, element_set_id);

            // TOFIX: Remove the assert after subdividing by materials
            // eventually it will be something like 
            //element_set[element_set_id][i] = element
            assert(element_set_id == 1);

            // TOFIX: not efficient
            elements.push_back(element);
        }
    }

    // sanity check
    assert(elements.size() == N_elements);

    // sanity check
    for (const auto & e : elements) {
        assert(e.sanityCheck());
    }

    // finalize
    ignoreRestOfLine(&fileStream);
    assert(fileStream.eof());
    fileStream.close();

    // free memory
    for (auto & vertex : vertices) {
        delete vertex;
    }

    // all done
    return true;
}

int main () {

    LoadMesh<mito::DIM2>("rectangle.summit");
    std::cout << "Loaded mesh" << std::endl;

    return 0;
}
