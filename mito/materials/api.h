// code guard
#if !defined(mito_materials_api_h)
#define mito_materials_api_h


namespace mito::materials {

    using gent_t = Gent;

    constexpr auto gent(real rho, real kappa, real mu, real Jm)
    {
        return gent_t(rho, kappa, mu, Jm);
    }

}


#endif

// end of file
