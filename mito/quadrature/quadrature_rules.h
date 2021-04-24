#ifndef __MITO__QUADRATURE__
#define __MITO__QUADRATURE__

#include "../mito.h"
#include "../mesh/simplex.h"

namespace mito {

    template <class quadrature_t, class element_t, int r>
    struct SampleQuadratureRule {
        static constexpr auto degreeExactness = r;
        static constexpr dim_t parametricDim = element_t::parametricDim;
        static constexpr auto Get();
    };

    struct GAUSS {
        static constexpr auto name = "GAUSS";
    };

    template <>
    constexpr auto SampleQuadratureRule<GAUSS, triangle_t, 1>::Get()
    {
        return std::array<std::tuple<vector<parametricDim>, double>, 1 /* nPoints */>(
            { { /*{point}, weight*/
                std::make_tuple(
                    vector<parametricDim>({ 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 }), 1.0 / 2.0) } });
    }

    template <>
    constexpr auto SampleQuadratureRule<GAUSS, triangle_t, 2>::Get()
    {
        return std::array<std::tuple<vector<parametricDim>, double>, 3 /* nPoints */>(
            { { /*{point}, weight*/
                std::make_tuple(
                    vector<parametricDim>({ 2.0 / 3.0, 1.0 / 6.0, 1.0 / 6.0 }), 1.0 / 6.0),
                std::make_tuple(
                    vector<parametricDim>({ 1.0 / 6.0, 2.0 / 3.0, 1.0 / 6.0 }), 1.0 / 6.0),
                std::make_tuple(
                    vector<parametricDim>({ 1.0 / 6.0, 1.0 / 6.0, 2.0 / 3.0 }), 1.0 / 6.0) } });
    }

    template <>
    constexpr auto SampleQuadratureRule<GAUSS, segment_t, 1>::Get()
    {
        return std::array<std::tuple<vector<parametricDim>, double>, 1 /* nPoints */>(
            { { /*{point}, weight*/
                std::make_tuple(vector<parametricDim>({ 1.0, 1.0 }), 2.0) } });
    }

    template <>
    constexpr auto SampleQuadratureRule<GAUSS, segment_t, 2>::Get()
    {
        return std::array<std::tuple<vector<parametricDim>, double>, 2 /* nPoints */>(
            { { /*{point}, weight*/
                std::make_tuple(
                    vector<parametricDim>({ sqrt(3.0) / 3.0, 1.0 - sqrt(3.0) / 3.0 }), 1.0),
                std::make_tuple(
                    vector<parametricDim>({ 1.0 - sqrt(3.0) / 3.0, sqrt(3.0) / 3.0 }), 1.0) } });
    }

}    // namespace mito

#endif    //__MITO__QUADRATURE__

// end of file
