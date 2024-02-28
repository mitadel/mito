// code guard
#if !defined(mito_quadrature_QuadratureTable_h)
#define mito_quadrature_QuadratureTable_h


namespace mito::quadrature {

    // a table to store quadrature rules in terms of their point-weight pairings
    template <class elementT, int Q>
    class Table {
      private:
        // the dimension of the parametric space
        static constexpr int parametricDim = manifolds::parametric_dim<elementT>();

      public:
        // the type of quadrature points' parametric coordinates
        using quadrature_point_type = manifolds::parametric_point_t<parametricDim>;
        // the type of quadrature weights
        using quadrature_weight_type = double;
        // the type of quadrature point-weight pairing
        using point_weight_pair_type = std::tuple<quadrature_point_type, quadrature_weight_type>;
        // the type for a collection of such pairings
        using array_type = std::array<point_weight_pair_type, Q>;

      public:
        // constructor
        constexpr Table(array_type && pairs) noexcept : _table{ std::move(pairs) } {}

        // the number of entries in the table (i.e. the pairings in the table)
        constexpr auto size() const -> int { return Q; }

        // get the q-th quadrature point
        constexpr auto point(int q) const -> quadrature_point_type
        {
            return std::get<0>(_table[q]);
        }

        // get the q-th quadrature weight
        constexpr auto weight(int q) const -> quadrature_weight_type
        {
            return std::get<1>(_table[q]);
        }

      private:
        // the table with the quadrature point-weight pairings
        const array_type _table;
    };

    // a factory for quadrature tables
    template <quadrature_formula quadratureT, class elementT, int r>
    constexpr auto QuadratureTable() -> auto;

}    // namespace mito


#include "quadrature_tables_segments.icc"
#include "quadrature_tables_triangles.icc"
#include "quadrature_tables_tetrahedra.icc"


#endif    // mito_quadrature_QuadratureTable_h

// end of file
