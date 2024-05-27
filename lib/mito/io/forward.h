// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    // class mesh writer
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(meshT::cell_type::dim == coordSystemT::coordinates_type::dim)
    class MeshWriter;

    // class point cloud writer
    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(cloudT::dim == coordSystemT::coordinates_type::dim)
    class PointCloudWriter;
}


// end of file
