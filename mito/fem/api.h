// code guard
#if !defined(mito_fem_api_h)
#define mito_fem_api_h


namespace mito::fem {

    template <int Q, class Y>
    using quadrature_field_t = QuadratureField<Q, Y>;

    template <int Q, class Y>
    constexpr auto quadrature_field(int nElements, std::string name = "")
    {
        return quadrature_field_t<Q, Y>(nElements, name);
    }

    template <typename T, int D>
    using nodal_field_t = NodalField<T, D>;

    template <typename T, int D>
    constexpr auto nodal_field(int nodes, std::string name = "")
    {
        return nodal_field_t<T, D>(nodes, name);
    }
}


#endif

// end of file
