// code guard
#if !defined(mito_fem_api_h)
#define mito_fem_api_h


namespace mito::fem {

    // quadrature field factory
    template <int Q, class Y>
    constexpr auto quadrature_field(int nElements, std::string name = "");

    // nodal field factory
    template <typename T, int D>
    constexpr auto nodal_field(int nodes, std::string name = "");

}


#endif

// end of file
