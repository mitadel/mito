#include "../mito.h"
#include "point.h"
#include <set>

namespace mito {

template <int D> 
class Simplex 
{
  public:
    Simplex(std::array< std::reference_wrapper<Simplex<D-1>>, D+1>&& entities) : 
        _entities(entities){}

    const auto & entities() const {return _entities;}

    void getVertices(std::set<const Simplex<0>* /* vertex_t* */>& vertices) const {
        for(const auto & entity : entities()) {
            entity.get().getVertices(vertices);
        }
    }

    bool sanityCheck() const {
        // check the subentities
        for(const auto & entity : entities()) {
            // if a subentity is broken, the sanity check fails
            if(!entity.get().sanityCheck()) {
                // all done
                return false;
            }
        }

        // use a set to cleanup duplicates
        std::set<const Simplex<0>* /* vertex_t* */> vertices;
        // collect vertices of every subentity of this simplex
        for(const auto & entity : entities()) {
            entity.get().getVertices(vertices);
        }
        // if this simplex does not have D+1 vertices, something went wrong
        if (vertices.size() != int(D) + 1) {
            // all done
            return false;
        }

        // all done
        return true;
    }

  private:
    std::array< std::reference_wrapper<Simplex<D-1>>, D+1> _entities;
};

template <> 
class Simplex<0> 
{
  public:
    Simplex(){};
    ~Simplex(){};

    void getVertices(std::set<const Simplex<0>* /* vertex_t* */>& vertices) const {
        // insert this vertex
        vertices.insert(this);
        // all done
        return; 
    }

    bool sanityCheck() const {
        return true;
    }

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

template<class element_t, DIM D>
void computeSimplicesVolume(const std::vector<element_t> & elements, 
    const VertexCoordinatesMap<D> &coordinatesMap, std::vector<real> & volumes) 
{
    // number of vertices
    constexpr int V = int(D)+1;

    static tensor<mito::DIM(V)> verticesTensor;

    // get number of elements
    int nElements = volumes.size();

    int countE = 0;
    for (const auto & e : elements) {
        std::fill(verticesTensor.begin(), verticesTensor.end(), 0.0);
        // use a set to collect vertices without repeated entries 
        std::set<const vertex_t*> vertices;
        e.getVertices(vertices);
        // assert you found D+1 vertices
        assert(V == vertices.size());

        int countV = 0;
        for(const auto & v : vertices) {
            for (auto d = 0; d < D; ++d) {
                verticesTensor[countV * V + d] = coordinatesMap[*v][d];
            }
            verticesTensor[countV * V + D] = 1.0;
            ++countV;
        }
        volumes[countE] = fabs(ComputeDeterminant(verticesTensor)) / Factorial<D>();
        ++countE;
    }
    // all done
    return;
}

template<class element_t, DIM D>
void computeElementsVolume(const std::vector<element_t> & elements, 
    const VertexCoordinatesMap<D> &coordinatesMap, std::vector<real> & volumes);

template<>
void computeElementsVolume<triangle_t, DIM2>(const std::vector<triangle_t> & elements, 
    const VertexCoordinatesMap<DIM2> &coordinatesMap, std::vector<real> & volumes)
{
    return computeSimplicesVolume<triangle_t, DIM2>(elements, coordinatesMap, volumes);
}

template<>
void computeElementsVolume<tetrahedron_t, DIM3>(const std::vector<tetrahedron_t> & elements, 
    const VertexCoordinatesMap<DIM3> &coordinatesMap, std::vector<real> & volumes)
{
    return computeSimplicesVolume<tetrahedron_t, DIM3>(elements, coordinatesMap, volumes);
}

template<class element_t, DIM D>
class ElementSet2 {

  public: 

    ElementSet2(std::vector<element_t> && elements, VertexCoordinatesMap<D> && coordinatesMap)
        : _elements(elements), _coordinatesMap(coordinatesMap), _jacobians(elements.size(), 0.0)
    {
        // compute the jacobians of the map from reference to current element for each element 
        _computeJacobians();

    }

    ~ElementSet2() {}

    bool sanityCheck() {
        bool check = true;
        for (const auto & e : _elements) {
            if(!e.sanityCheck()) {
                std::cout << "Failed sanity check for element " << e << std::endl;
                check = false;
            }
        }
        return check;
    }

  private:
    void _computeJacobians() {
        return computeElementsVolume<element_t /* element type */, D /* spatial dim*/>
            (_elements, _coordinatesMap, _jacobians);
    }

  private:    
    const std::vector<element_t> _elements;
    const VertexCoordinatesMap<D> _coordinatesMap;
    std::vector<real> _jacobians;
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
