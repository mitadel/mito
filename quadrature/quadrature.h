#include "../mito.h"
#include <vector>

namespace mito {

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

    // NOTE: The number of vertices coincides with d, is it always the case?
    inline void quadraturePointsCurrentElement(const std::vector<mito::vector<D> > & vertices, 
        std::vector<mito::vector<D> > & coordinates) const {

        assert (coordinates.size() == _points.size());

        for (size_t i = 0; i < _points.size(); ++i) {
            for (size_t j = 0; j < D; ++j) {
                coordinates[i][j] = 0.0;
                for (size_t a = 0; a < vertices.size(); ++a) {
                    coordinates[i][j] += _points[i][j]*vertices[a][j];
                }
            }
        }

        // all done
        return;
    } 

  protected:
    std::vector<double> _weights;
    std::vector<mito::vector<d> > _points;
};

class QuadRuleTri1 : public QuadRule<DIM3, DIM2> {
  public:

    // Triangle order 1
    QuadRuleTri1() : QuadRule<DIM3, DIM2>({1.0 / 2.0} /*weights*/, 
        {{1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0}} /*points*/) {}

    ~QuadRuleTri1() {}

};

}  // namespace mito

// end of file
