// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::summit {

    template <class cellT, geometry::coordinates_c coordT>
    requires(utilities::same_dim_c<cellT, coordT>)
    auto writer(
        std::string filename, const mito::mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordT> & coordinate_system) -> void
    {
        // create a writer
        auto mesh_writer = mesh_writer_t(filename, mesh, coordinate_system);
        // write
        return mesh_writer.write();
    }

}    // namespace mito::io::summit


// end of file
