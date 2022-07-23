// -*- C++ -*-
//
// michael a.g. aïvázis
// orthologue
// (c) 1998-2022 all rights reserved
//

// code guard
#if !defined(pyre_mpi_Handle_h)
#define pyre_mpi_Handle_h

// place Handle in namespace pyre::mpi
namespace pyre {
    namespace mpi {
        template <typename Resource> class Handle;
    }
}


// declaration
template <typename Resource>
class pyre::mpi::Handle {
    // types
public:
    using resource_t = Resource;
    using handle_t = Resource::handle_t;
    using shared_t = Resource::shared_t;

    // interface
public:
    inline operator handle_t() const;

    // meta methods
public:
    inline ~Handle();
    inline Handle(handle_t, bool);
    inline Handle(const Handle &);
    inline Handle & operator=(const Handle &);

    // data members
private:
    shared_t * _shared;
};


// get the inline definitions
#define pyre_mpi_Handle_icc
#include "Handle.icc"
#undef pyre_mpi_Handle_icc


# endif
// end of file
