#include "../mito.h"
#include "point.h"

namespace mito {

template <int D> 
class Simplex 
{
  public:
    Simplex(std::array< std::reference_wrapper<Simplex<D-1>>, D+1>&& entities) : 
        _entities(entities){}

    const auto & entities() const {return _entities;}


  private:
    std::array< std::reference_wrapper<Simplex<D-1>>, D+1> _entities;
};

template <> 
class Simplex<0> 
{
  public:
    Simplex(){};
    ~Simplex(){};
};

template <int D> 
class OrientedSimplex : public Simplex<D>
{
  public:
    OrientedSimplex(std::array< std::reference_wrapper<Simplex<D-1>>, D+1>&& entities, 
        bool orientation) : Simplex<D>(entities), _orientation(orientation) {}

  private:
    bool _orientation;    
};

using vertex_t = Simplex<0>;
using segment_t = Simplex<1>;
using triangle_t = Simplex<2>;
using tetrahedron_t = Simplex<3>;

template<DIM D>
// QUESTION: can we call this 'connectivity'? 
class VertexCoordinatesMap {

    //using map_t = std::unordered_map<std::reference_wrapper<vertex_t>, point_t<D>>;
    // TODO: test O for access
    using map_t = std::unordered_map<const vertex_t*, const point_t<D>*>;

  public:
    VertexCoordinatesMap() : _map() {};

    auto insert(const vertex_t & vertex, const point_t<D>&& point) {
        return _map.insert(
            //std::pair<std::reference_wrapper<vertex_t>, point_t<D>>(vertex, point)
            std::pair<const vertex_t*, const point_t<D>*>(&vertex, &point)
        );
    }

    const point_t<D>& operator[](const vertex_t & vertex) const {
        auto point = _map.find(&vertex);
        return *(point->second);
    }

  private:
    map_t _map;
};

}

// overload operator<< for simplices
template<int D>
std::ostream& operator<<(std::ostream& os, const mito::Simplex<D>& s)
{
    os << &s << " composed of:" << std::endl;
    for(const auto & entity : s.entities()) {
        std::cout << "\t" << entity.get() << std::endl;
    }
    return os;
}

// overload operator<< specialization for simplices with D = 0 (vertices)
template<>
std::ostream& operator<<(std::ostream& os, const mito::Simplex<0>& s)
{
    os << &s;
    return os;
}
