// code guard
#if !defined(mito_topology_factories_h)
#define mito_topology_factories_h


namespace mito::topology {

    // topology factory
    auto topology() -> topology_t &
    {
        return utilities::Singleton<topology_t>::GetInstance();
    }
}

#endif

// end of file
