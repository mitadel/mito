// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    // class mesh writer
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    class MeshWriterVTK;

    // class point cloud writer
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    class PointCloudWriterVTK;

    // class field writer
    template <class gridWriterT, geometry::coordinate_system_c coordSystemT>
    class FieldWriterVTK;
}


// end of file
