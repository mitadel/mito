// code guard
#if !defined(mito_topology_factories_h)
#define mito_topology_factories_h


namespace mito::topology {

    // topology factory
    auto topology() -> topology_t &
    {
        return mito::utilities::Singleton<topology_t>::GetInstance();
    }
}

#endif

// end of file
