// code guard
#if !defined(mito_materials_api_h)
#define mito_materials_api_h


namespace mito::materials {

    // gent material factory
    constexpr auto gent(real rho, real kappa, real mu, real Jm) -> gent_t;
}


#endif

// end of file
