// code guard
#if !defined(mito_quadrature_QuadratureRule_h)
#define mito_quadrature_QuadratureRule_h


namespace mito::quadrature {

    // quadrature point representation in parametric coordinates
    template <int D>
    using quadrature_point_t = mito::manifolds::parametric_point_t<D>;

    template <class quadrature_t, class element_t, int r>
    struct QuadratureRulesFactory {
        static constexpr auto degreeExactness = r;
        static constexpr int parametricDim = mito::manifolds::parametric_dim<element_t>();
        static constexpr auto Get() -> auto;

      private:
        static constexpr auto GetQuadratureRule() -> auto;
    };

    struct GAUSS {
        static constexpr auto name = "GAUSS";
    };

    template <int parametricDim, int Q>
    class quadrature_array_t :
        public std::array<std::tuple<quadrature_point_t<parametricDim>, double>, Q> {
      public:
        template <typename... T>
        constexpr quadrature_array_t(T &&... t) :
            std::array<std::tuple<quadrature_point_t<parametricDim>, double>, Q> { std::forward<T>(
                t)... }
        {}

        constexpr auto getPoint(int q) const { return std::get<0>((*this)[q]); }
        constexpr auto getWeight(int q) const { return std::get<1>((*this)[q]); }
    };

    template <class quadrature_t, class element_t, int r>
    constexpr auto QuadratureRulesFactory<quadrature_t, element_t, r>::Get() -> auto
    {
        // get the appropriate quadrature rule
        constexpr auto quadrature_rule =
            QuadratureRulesFactory<quadrature_t, element_t, r>::GetQuadratureRule();

        // lambda function to compute the sum of Q quadrature weights at compile time
        constexpr auto sum = [quadrature_rule]<int Q>() consteval->double
        {
            constexpr auto sum_impl = [quadrature_rule]<int q>(auto & sum_ref) consteval->double
            {
                if constexpr (q == -1) {
                    return 0.0;
                } else {
                    return quadrature_rule.getWeight(q)
                         + sum_ref.template operator()<q - 1>(sum_ref);
                }
            };
            return sum_impl.template operator()<Q - 1>(sum_impl);
        };

        // the number of quadrature weights
        constexpr auto Q = quadrature_rule.size();
        // have the compiler compute the sum of the quadrature weights
        constexpr double weightsSum = sum.template operator()<Q>();
        // assert the quadrature weights are a partition of unity
        static_assert(weightsSum == 1.0);

        // return the quadrature rule
        return quadrature_rule;
    }

    template <>
    constexpr auto QuadratureRulesFactory<GAUSS, topology::triangle_t, 1>::GetQuadratureRule()
        -> auto
    {
        return quadrature_array_t<parametricDim, 1 /* nPoints */>(
            { /*{point}, weight*/
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 }), 1.0) });
    }

    template <>
    constexpr auto QuadratureRulesFactory<GAUSS, topology::triangle_t, 2>::GetQuadratureRule()
        -> auto
    {
        return quadrature_array_t<parametricDim, 3 /* nPoints */>(
            { /*{point}, weight*/
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 2.0 / 3.0, 1.0 / 6.0, 1.0 / 6.0 }),
                  1.0 / 3.0),
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 1.0 / 6.0, 2.0 / 3.0, 1.0 / 6.0 }),
                  1.0 / 3.0),
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 1.0 / 6.0, 1.0 / 6.0, 2.0 / 3.0 }),
                  1.0 / 3.0) });
    }

    template <>
    constexpr auto QuadratureRulesFactory<GAUSS, topology::segment_t, 1>::GetQuadratureRule()
        -> auto
    {
        return quadrature_array_t<parametricDim, 1 /* nPoints */>(
            { /*{point}, weight*/
              std::make_tuple(quadrature_point_t<parametricDim>({ 1.0, 1.0 }), 1.0) });
    }

    template <>
    constexpr auto QuadratureRulesFactory<GAUSS, topology::segment_t, 2>::GetQuadratureRule()
        -> auto
    {
        return quadrature_array_t<parametricDim, 2 /* nPoints */>(
            { /*{point}, weight*/
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 0.788675134594813, 0.211324865405187 }), 0.5),
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 0.211324865405187, 0.788675134594813 }),
                  0.5) });
    }

    template <>
    constexpr auto QuadratureRulesFactory<GAUSS, topology::tetrahedron_t, 1>::GetQuadratureRule()
        -> auto
    {
        /*Hammer rule*/
        return quadrature_array_t<parametricDim, 1 /* nPoints */>(
            { /*{point}, weight*/
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 1.0 / 4.0, 1.0 / 4.0, 1.0 / 4.0, 1.0 / 4.0 }),
                  1.0) });
    }

    template <>
    constexpr auto QuadratureRulesFactory<GAUSS, topology::tetrahedron_t, 2>::GetQuadratureRule()
        -> auto
    {
        /*Hammer rule*/
        return quadrature_array_t<parametricDim, 4 /* nPoints */>(
            { /*{point}, weight*/
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 0.5854101966249685e0, 0.1381966011250105e0,
                                                      0.1381966011250105e0, 0.1381966011250105e0 }),
                  1.0 / 4.0),
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 0.1381966011250105e0, 0.5854101966249685e0,
                                                      0.1381966011250105e0, 0.1381966011250105e0 }),
                  1.0 / 4.0),
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 0.1381966011250105e0, 0.1381966011250105e0,
                                                      0.5854101966249685e0, 0.1381966011250105e0 }),
                  1.0 / 4.0),
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 0.1381966011250105e0, 0.1381966011250105e0,
                                                      0.1381966011250105e0, 0.5854101966249685e0 }),
                  1.0 / 4.0) });
    }

}    // namespace mito

#endif    // mito_quadrature_QuadratureRule_h

// end of file
