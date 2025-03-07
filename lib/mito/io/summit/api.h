// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::summit {

    // mesh writer alias
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    using mesh_writer_t = MeshSummitWriter<meshT, coordSystemT>;

    // the type of mesh (either CG or DG)
    enum GalerkinMeshType { CG, DG };
}


// end of file
