// code guard
#if !defined(mito_quadrature_QuadratureRule_h)
#define mito_quadrature_QuadratureRule_h


namespace mito::quadrature {

    // quadrature point representation in parametric coordinates
    template <int D>
    using quadrature_point_t = manifolds::parametric_point_t<D>;

    // the quadrature weights type
    using quadrature_weight_t = double;

    // the factory for quadrature rules
    template <class quadrature_t, class element_t, int r>
    struct QuadratureRulesFactory {
        // the highest polynomial degree integrated exactly by the quadrature rule
        static constexpr auto degreeExactness = r;
        // the parametric dimension of the element type {element_t}
        static constexpr int parametricDim = manifolds::parametric_dim<element_t>();
        // get the quadrature rule
        static constexpr auto Get() -> auto;

      private:
        static constexpr auto GetQuadratureRule() -> auto;
    };

    struct GAUSS {
        static constexpr auto name = "GAUSS";
    };

    template <int parametricDim, int Q>
    class QuadratureRule {
      private:
        using quadrature_point_type = quadrature_point_t<parametricDim>;
        using quadrature_weight_type = quadrature_weight_t;
        using point_weight_pair_type = std::tuple<quadrature_point_type, quadrature_weight_type>;
        using array_type = std::array<point_weight_pair_type, Q>;

      public:
        static constexpr int npoints = Q;

      public:
        constexpr QuadratureRule(array_type && t) : _rule(t) {}

        constexpr auto point(int q) const -> quadrature_point_type { return std::get<0>(_rule[q]); }
        constexpr auto weight(int q) const -> quadrature_weight_type
        {
            return std::get<1>(_rule[q]);
        }

        const array_type _rule;
    };

    template <int parametricDim, int Q>
    using quadrature_rule_t = QuadratureRule<parametricDim, Q>;

    template <class quadrature_t, class element_t, int r>
    constexpr auto QuadratureRulesFactory<quadrature_t, element_t, r>::Get() -> auto
    {
        // get the appropriate quadrature rule
        constexpr auto quadrature_rule =
            QuadratureRulesFactory<quadrature_t, element_t, r>::GetQuadratureRule();

        // lambda function to compute the sum of Q quadrature weights at compile time
        constexpr auto sum = [quadrature_rule]<int Q>() consteval -> quadrature_weight_t {
            constexpr auto sum_impl =
                [quadrature_rule]<int q>(auto & sum_ref) consteval -> quadrature_weight_t {
                if constexpr (q == -1) {
                    return 0.0;
                } else {
                    return quadrature_rule.weight(q) + sum_ref.template operator()<q - 1>(sum_ref);
                }
            };
            return sum_impl.template operator()<Q - 1>(sum_impl);
        };

        // the number of quadrature weights
        constexpr auto Q = decltype(quadrature_rule)::npoints;
        // have the compiler compute the sum of the quadrature weights
        constexpr auto weightsSum = sum.template operator()<Q>();
        // assert the quadrature weights are a partition of unity
        static_assert(weightsSum == 1.0);

        // return the quadrature rule
        return quadrature_rule;
    }

    template <>
    constexpr auto QuadratureRulesFactory<GAUSS, topology::triangle_t, 1>::GetQuadratureRule()
        -> auto
    {
        return quadrature_rule_t<parametricDim, 1 /* nPoints */>(
            { /*{point}, weight*/
              std::make_tuple(
                  quadrature_point_t<parametricDim>({ 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 }), 1.0) });
    }

    template <>
    constexpr auto QuadratureRulesFactory<GAUSS, topology::triangle_t, 2>::GetQuadratureRule()
        -> auto
    {
        return quadrature_rule_t<parametricDim, 3 /* nPoints */>(
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
    constexpr auto QuadratureRulesFactory<GAUSS, topology::triangle_t, 3>::GetQuadratureRule()
        -> auto
    {
        constexpr quadrature_weight_t a = 0.16288285039589191090016180418490635;
        constexpr quadrature_weight_t b = 0.47791988356756370000000000000000000;

        constexpr quadrature_weight_t w1 = 0.28114980244097964825351432270207695;
        constexpr quadrature_weight_t w2 = 0.05218353089235368507981901063125638;

        return quadrature_rule_t<parametricDim, 6 /* nPoints */>(
            { /*{point}, weight*/
              std::make_tuple(quadrature_point_t<parametricDim>({ 1.0 - 2.0 * a, a, a }), w1),
              std::make_tuple(quadrature_point_t<parametricDim>({ a, 1.0 - 2.0 * a, a }), w1),
              std::make_tuple(quadrature_point_t<parametricDim>({ a, a, 1.0 - 2.0 * a }), w1),
              std::make_tuple(quadrature_point_t<parametricDim>({ 1.0 - 2.0 * b, b, b }), w2),
              std::make_tuple(quadrature_point_t<parametricDim>({ b, 1.0 - 2.0 * b, b }), w2),
              std::make_tuple(quadrature_point_t<parametricDim>({ b, b, 1.0 - 2.0 * b }), w2) });
    }

    template <>
    constexpr auto QuadratureRulesFactory<GAUSS, topology::segment_t, 1>::GetQuadratureRule()
        -> auto
    {
        return quadrature_rule_t<parametricDim, 1 /* nPoints */>(
            { /*{point}, weight*/
              std::make_tuple(quadrature_point_t<parametricDim>({ 1.0, 1.0 }), 1.0) });
    }

    template <>
    constexpr auto QuadratureRulesFactory<GAUSS, topology::segment_t, 2>::GetQuadratureRule()
        -> auto
    {
        return quadrature_rule_t<parametricDim, 2 /* nPoints */>(
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
        return quadrature_rule_t<parametricDim, 1 /* nPoints */>(
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
        return quadrature_rule_t<parametricDim, 4 /* nPoints */>(
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
