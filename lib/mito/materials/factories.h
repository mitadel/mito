// code guard
#if !defined(mito_materials_factories_h)
#define mito_materials_factories_h


namespace mito::materials {

    // gent material factory
    constexpr auto gent(real rho, real kappa, real mu, real Jm)
    {
        return gent_t(rho, kappa, mu, Jm);
    }
}


#endif

// end of file