#include "../mito.h"

template <int D> 
class Simplex 
{
  public:
    Simplex(std::array< std::reference_wrapper<Simplex<D-1>>, D+1>&& entities) : 
        _entities(entities){}

    const auto & entities() const {return _entities;}

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

// overload operator<< for simplices
template<int D>
std::ostream& operator<<(std::ostream& os, const Simplex<D>& s)
{
    os << &s << " composed of:" << std::endl;
    for(const auto & entity : s.entities()) {
        std::cout << "\t" << entity.get() << std::endl;
    }
    return os;
}

// overload operator<< specialization for simplices with D = 0 (vertices)
template<>
std::ostream& operator<<(std::ostream& os, const Simplex<0>& s)
{
    os << &s;
    return os;
}

using vertex_t = Simplex<0>;
using segment_t = Simplex<1>;
using triangle_t = Simplex<2>;
using tetrahedron_t = Simplex<3>;
