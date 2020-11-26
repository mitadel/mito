#ifndef __MITO__QUADRATURE__
#define __MITO__QUADRATURE__

#include "../mito.h"

namespace mito {

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

    inline std::vector<mito::vector<D> > quadraturePointsCurrentElement(
        const std::vector<mito::vector<D> > & vertices) const {

        // NOTE: The number of vertices coincides with d, is it always the case?
        assert(vertices.size() == d);

        std::vector<mito::vector<D> > coordinates(_quadPoints.size());

        for (size_t i = 0; i < _quadPoints.size(); ++i) {
            for (size_t j = 0; j < D; ++j) {
                coordinates[i][j] = 0.0;
                for (size_t a = 0; a < vertices.size(); ++a) {
                    coordinates[i][j] += _quadPoints[i][j]*vertices[a][j];
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

    // Cowper quadrature rule (1-point formula)
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
    return std::move(quadrule);
};

template<>
QuadRule<parametricDim<TRI>(), physicalDim<TRI>() > QuadratureRule<TRI, 2>() {
    QuadRuleTri2 quadrule; 
    return std::move(quadrule);
};

}  // namespace mito

#endif //__MITO__QUADRATURE__

// end of file
