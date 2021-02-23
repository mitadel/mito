#ifndef __MITO__QUADRATURE__
#define __MITO__QUADRATURE__

#include "../mito.h"
#include "../elements/elements.h"

namespace mito {

template <class ElementType, int Q /* number of quadrature points */>
class QuadRule {

    // the dimension D of the physical space of the integration domain 
    static const DIM D = ElementType::physicalDim;
    // the dimension d of the parameteric space on the reference element 
    static const DIM d = ElementType::parametricDim;

  public:
    QuadRule(const std::array<double, Q> && quadWeights, 
        const std::array<mito::vector<d>, Q> && quadPoints) 
        : _quadWeights(quadWeights), _quadPoints(quadPoints) {}

    virtual ~QuadRule() {}; 

    // accessors
    inline int nQuad() const {
        return Q;
    }

    inline double weight(int i) const {
        assert(i < Q);
        return _quadWeights[i];
    }

    inline const mito::vector<d>& point(int i) const {
        assert(i < Q);
        return _quadPoints[i];
    }

    inline const std::array<double, Q>& weights() const {
        return _quadWeights;
    }

    inline const std::array<mito::vector<d>, Q>& points() const {
        return _quadPoints;
    }

    // TOFIX: Methods quadraturePointsCurrentElement and quadraturePoints are actually mapping from 
    //        the reference element to the current element. This mapping should not be computed by 
    //        the quadrature rule nor by the elements, but should be a helper global function using
    //        the public interface of the quadrature rule and of the elements class.
    inline std::vector<mito::vector<D> > quadraturePointsCurrentElement(
        const std::vector<mito::vector<D> > & vertices) const {

        // NOTE: The number of vertices coincides with d. This is the case for simplices but it 
        //       might not always be the case.
        assert(vertices.size() == d);

        std::vector<mito::vector<D> > coordinates(_quadPoints.size());

        for (auto i = 0; i < _quadPoints.size(); ++i) {
            for (auto j = 0; j < D; ++j) {
                coordinates[i][j] = 0.0;
                for (auto a = 0; a < vertices.size(); ++a) {
                    coordinates[i][j] += _quadPoints[i][a]*vertices[a][j];
                }
            }
        }

        // all done
        return std::move(coordinates);
    } 

    // TOFIX: This should return an ElementQuadratureField. But how do quadrature fields fit in now 
    //        that we have completely decoupled the integration from the element sets?

    inline std::vector<mito::vector<D> > 
        quadraturePoints(const Elements<ElementType> & elements) const {

        std::vector<mito::vector<D> > coordinates(elements.nElements() * _quadPoints.size());

        // TOFIX: We should avoid the 4 nested for loops
        for (auto e = 0; e < elements.nElements(); ++e) {
            for (auto i = 0; i < Q; ++i) {
                for (auto j = 0; j < D; ++j) {
                    coordinates[e * _quadPoints.size() + i][j] = 0.0;
                    for (auto v = 0; v < elements.nVertices(); ++v) {
                        const mito::vector<D> & vertex = elements.vertex(e, v);
                        coordinates[e * _quadPoints.size() + i][j] 
                            += _quadPoints[i][v]*vertex[j];
                    }
                }
            }
        }

        // all done
        return std::move(coordinates);
    }

  protected:
    std::array<double, Q> _quadWeights;
    std::array<mito::vector<d>, Q> _quadPoints;
};

// Helper function to return the number of quadrature points of a quadrature rule based on the 
// element type and the degree of exactness
template<class ElementType, int r> 
constexpr int nquad();

template<>
constexpr int nquad<TRI, 1 /* degree of exactness */>() {return 1;}

template<>
constexpr int nquad<TRI, 2 /* degree of exactness */>() {return 3;}

// Factory function to instantiate quadrature rules
template<class ElementType, int r>
QuadRule<ElementType, nquad<ElementType, r>() > QuadratureRule();

template<>
QuadRule<TRI, 1 /* Q */> 
QuadratureRule<TRI, 1 /* degree of exactness */>() {
    // Triangle order 1
    QuadRule<TRI, 1 /* Q */ > quadrule(
        {{/*weights*/
            1.0 / 2.0
        }},
        {{/*points*/
            {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0}
        }});
    return quadrule;
};

template<>
QuadRule<TRI, 3 /* Q */> 
QuadratureRule<TRI, 2 /* degree of exactness */>() {
    // Triangle order 2
    QuadRule<TRI, 3 /* Q */ > quadrule(
        {{/*weights*/ 
            1.0 / 6.0, 
            1.0 / 6.0, 
            1.0 / 6.0
        }},
        {{/*points*/
            {2.0 / 3.0, 1.0 / 6.0, 1.0 / 6.0},
            {1.0 / 6.0, 2.0 / 3.0, 1.0 / 6.0},
            {1.0 / 6.0, 1.0 / 6.0, 2.0 / 3.0}
        }});
    return quadrule;
};

}  // namespace mito

#endif //__MITO__QUADRATURE__

// end of file
