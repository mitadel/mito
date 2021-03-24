#include "../mito.h"

template <int D> 
class Simplex 
{
  public:
    Simplex(std::array< std::reference_wrapper<Simplex<D-1>>, D+1>&& entities) : 
        _entities(entities){}

    void print() {
        for(const auto & entity : _entities) {
            std::cout << &entity << std::endl;
        }
    }

  public:
    std::array< std::reference_wrapper<Simplex<D-1>>, D+1> _entities;
};

template <> 
class Simplex<0> 
{
  public:
    Simplex(){};
    ~Simplex(){};
};

using vertex_t = Simplex<0>;
using segment_t = Simplex<1>;
using triangle_t = Simplex<2>;
using tetrahedron_t = Simplex<3>;
