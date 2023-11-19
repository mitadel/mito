// code guard
#if !defined(mito_quadrature_QuadratureRule_h)
#define mito_quadrature_QuadratureRule_h


namespace mito::quadrature {

    template <quadrature_formula quadratureT, class elementT, int r>
    class QuadratureRule {
      private:
        // the table with the quadrature points and weights
        static constexpr auto table = QuadratureTable<quadratureT, elementT, r>();
        // the table type
        using table_type = decltype(table);
        // the type of quadrature points in the table
        using quadrature_point_type = table_type::quadrature_point_type;
        // the type of quadrature weights in the table
        using quadrature_weight_type = table_type::quadrature_weight_type;

      public:
        // the number of point-weight pairs
        static constexpr int npoints = std::size(table);

      public:
        // constructor
        constexpr QuadratureRule() { _check_weights(); }

        // get the q-th point in the table
        constexpr auto point(int q) const -> quadrature_point_type { return table.point(q); }

        // get the q-th weight in the table
        constexpr auto weight(int q) const -> quadrature_weight_type { return table.weight(q); }

      private:
        // helper function to check that the weights add up to 1.0
        constexpr auto _check_weights()
        {
            // lambda function to compute the sum of Q quadrature weights at compile time
            constexpr auto sum =
                []<int Q>(const table_type & table) consteval -> quadrature_weight_type {
                constexpr auto sum_impl = []<int q>(
                                              const table_type & table,
                                              auto & sum_ref) consteval -> quadrature_weight_type {
                    if constexpr (q == -1) {
                        return 0.0;
                    } else {
                        return table.weight(q) + sum_ref.template operator()<q - 1>(table, sum_ref);
                    }
                };
                return sum_impl.template operator()<Q - 1>(table, sum_impl);
            };

            // have the compiler compute the sum of the quadrature weights
            constexpr auto weightsSum = sum.template operator()<npoints>(table);
            // assert the quadrature weights are a partition of unity
            static_assert(weightsSum == 1.0);

            // all done
            return;
        }
    };

}    // namespace mito

#endif    // mito_quadrature_QuadratureRule_h

// end of file
