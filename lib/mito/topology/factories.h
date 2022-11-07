// code guard
#if !defined(mito_topology_factories_h)
#define mito_topology_factories_h


namespace mito::topology {

    // topology factory
    auto topology() -> mito::topology::topology_t
    {
        return mito::topology::topology_t();
    }
}

#endif

// end of file
