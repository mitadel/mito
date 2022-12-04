// code guard
#if !defined(mito_topology_factories_h)
#define mito_topology_factories_h


namespace mito::topology {

    // topology factory
    auto topology() -> mito::topology::topology_t &
    {
        return mito::topology::TopologySingleton::GetInstance();
    }
}

#endif

// end of file
