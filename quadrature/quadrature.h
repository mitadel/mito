#ifndef __MITO__QUADRATURE__
#define __MITO__QUADRATURE__

#include "../mito.h"
#include "../elements/elements.h"

namespace mito {

template<class QuadratureType, class ElementType, int r>
struct SampleQuadratureRule
{
    static constexpr auto degreeExactness = r;
    static constexpr auto parametricDim = ElementType::parametricDim;
    static constexpr auto Get();
};

struct GAUSS {
    static constexpr auto name = "GAUSS";
};

template<>
constexpr auto SampleQuadratureRule<GAUSS, TRI, 1>::Get() {
    return 
    std::array<std::tuple<vector<parametricDim>, double>, 1 /* nPoints */>(
    {{/*{point}, weight*/
        std::make_tuple(vector<parametricDim>({1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0}), 1.0 / 2.0)
    }});
}

template<>
constexpr auto SampleQuadratureRule<GAUSS, TRI, 2>::Get() {
    return 
    std::array<std::tuple<vector<parametricDim>, double>, 3 /* nPoints */>(
    {{    /*{point}, weight*/
        std::make_tuple(vector<parametricDim>({2.0 / 3.0, 1.0 / 6.0, 1.0 / 6.0}), 1.0 / 6.0),
        std::make_tuple(vector<parametricDim>({1.0 / 6.0, 2.0 / 3.0, 1.0 / 6.0}), 1.0 / 6.0),
        std::make_tuple(vector<parametricDim>({1.0 / 6.0, 1.0 / 6.0, 2.0 / 3.0}), 1.0 / 6.0)
    }});
}

}  // namespace mito

#endif //__MITO__QUADRATURE__

// end of file
