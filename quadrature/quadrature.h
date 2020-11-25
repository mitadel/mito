#include "../mito.h"

namespace mito {

// template with respect to the dimension d of the parameteric space on the reference element 
// and on the dimension D of the physical space of the integration domain 
template <DIM d, DIM D>
class QuadRule {
  public:
    QuadRule(const std::vector<double> && weights, const std::vector<mito::vector<d> > && points) : 
        _weights(weights), _points(points) {}
    virtual ~QuadRule() {}; 

    // accessors
    inline int npoints() const {
        return _weights.size();
    }

    inline double weight(size_t i) const {
        assert(i < _weights.size());
        return _weights[i];
    }

    inline const mito::vector<d>& point(size_t i) const {
        assert(i < _points.size());
        return _points[i];
    }

    inline const std::vector<double>& weights() const {
        return _weights;
    }

    inline const std::vector<mito::vector<d> >& points() const {
        return _points;
    }

    inline std::vector<mito::vector<D> > quadraturePointsCurrentElement(
        const std::vector<mito::vector<D> > & vertices) const {

        // NOTE: The number of vertices coincides with d, is it always the case?
        assert(vertices.size() == d);

        std::vector<mito::vector<D> > coordinates(_points.size());

        for (size_t i = 0; i < _points.size(); ++i) {
            for (size_t j = 0; j < D; ++j) {
                coordinates[i][j] = 0.0;
                for (size_t a = 0; a < vertices.size(); ++a) {
                    coordinates[i][j] += _points[i][j]*vertices[a][j];
                }
            }
        }

        // all done
        return std::move(coordinates);
    } 

  protected:
    std::vector<double> _weights;
    std::vector<mito::vector<d> > _points;
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

}  // namespace mito

// end of file
