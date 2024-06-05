// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    // class mesh writer
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(meshT::dim == coordSystemT::dim)
    class MeshWriterVTK;

    // class point cloud writer
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(cloudT::dim == coordSystemT::dim)
    class PointCloudWriterVTK;

    // class field writer
    template <class gridT, geometry::coordinate_system_c coordSystemT>
    class FieldWriterVTK;
}


// end of file
