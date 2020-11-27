#ifndef __MITO__QUADRATURE__
#define __MITO__QUADRATURE__

#include "../mito.h"
#include "../elements/elements.h"

namespace mito {

// TOFIX: Consider introducing a reference element class. Both the quadrule and the elements class 
//        can be templated on it, so as to guarantee compatibility between quadrule for the 
//        integration and the domain of integration at compile time. 

// template with respect to the dimension d of the parameteric space on the reference element 
// and on the dimension D of the physical space of the integration domain 
template <DIM d, DIM D>
class QuadRule {
  public:
    QuadRule(const std::vector<double> && quadWeights, 
        const std::vector<mito::vector<d> > && quadPoints) 
        : _quadWeights(quadWeights), _quadPoints(quadPoints) {}
    virtual ~QuadRule() {}; 

    // accessors
    inline int nQuad() const {
        return _quadWeights.size();
    }

    inline double weight(size_t i) const {
        assert(i < _quadWeights.size());
        return _quadWeights[i];
    }

    inline const mito::vector<d>& point(size_t i) const {
        assert(i < _quadPoints.size());
        return _quadPoints[i];
    }

    inline const std::vector<double>& weights() const {
        return _quadWeights;
    }

    inline const std::vector<mito::vector<d> >& points() const {
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

        for (size_t i = 0; i < _quadPoints.size(); ++i) {
            for (size_t j = 0; j < D; ++j) {
                coordinates[i][j] = 0.0;
                for (size_t a = 0; a < vertices.size(); ++a) {
                    coordinates[i][j] += _quadPoints[i][a]*vertices[a][j];
                }
            }
        }

        // all done
        return std::move(coordinates);
    } 

    // TOFIX: This should return an ElementQuadratureField. But how do quadrature fields fit in now 
    //        that we have completely decoupled the integration from the element sets?

    inline std::vector<mito::vector<D> > quadraturePoints(const Elements<D> & elements) const {

        std::vector<mito::vector<D> > coordinates(elements.nElements() * _quadPoints.size());

        // TOFIX: We should avoid the 4 nested for loops
        for (size_t e = 0; e < elements.nElements(); ++e) {
            for (size_t i = 0; i < _quadPoints.size(); ++i) {
                for (size_t j = 0; j < D; ++j) {
                    coordinates[e * _quadPoints.size() + i][j] = 0.0;
                    for (size_t v = 0; v < elements.nVertices(); ++v) {
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
    std::vector<double> _quadWeights;
    std::vector<mito::vector<d> > _quadPoints;
};

// TOFIX: QuadRuleTri1 and QuadRuleTri2 should not be classes deriving from QuadRule<DIM3, DIM2> 
//        but rather instances of QuadRule<DIM3, DIM2>.  

// Triangle order 1
class QuadRuleTri1 : public QuadRule<DIM3, DIM2> {
  public:

    // Cowper quadrature rule (1-point formula)
    QuadRuleTri1() : QuadRule<DIM3, DIM2>(
        {/*weights*/
            1.0 / 2.0
        },
        {/*points*/
            {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0}
        }
        ) {}

    ~QuadRuleTri1() {}

};

// Triangle order 2
class QuadRuleTri2 : public QuadRule<DIM3, DIM2> {
  public:

    // Cowper quadrature rule (3-point formula)
    QuadRuleTri2() : QuadRule<DIM3, DIM2>(
        {/*weights*/ 
            1.0 / 6.0, 
            1.0 / 6.0, 
            1.0 / 6.0
        },
        {/*points*/
            {2.0 / 3.0, 1.0 / 6.0, 1.0 / 6.0},
            {1.0 / 6.0, 2.0 / 3.0, 1.0 / 6.0},
            {1.0 / 6.0, 1.0 / 6.0, 2.0 / 3.0}
        }
        ) {}

    ~QuadRuleTri2() {}

};

// Factory function to instantiate quadrature rules
template<ElementType T, size_t r>
QuadRule<parametricDim<T>(), physicalDim<T>() > QuadratureRule();

template<>
QuadRule<parametricDim<TRI>(), physicalDim<TRI>() > QuadratureRule<TRI, 1>() {
    QuadRuleTri1 quadrule; 
    return quadrule;
};

template<>
QuadRule<parametricDim<TRI>(), physicalDim<TRI>() > QuadratureRule<TRI, 2>() {
    QuadRuleTri2 quadrule; 
    return quadrule;
};

}  // namespace mito

#endif //__MITO__QUADRATURE__

// end of file
