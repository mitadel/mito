// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    // class grid writer
    template <int D>
    class GridVTKWriter;

    // class mesh writer
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT, class vtkGridWriterT>
    requires(
        utilities::same_dim_c<meshT, coordSystemT> && utilities::same_dim_c<meshT, vtkGridWriterT>)
    class MeshVTKWriter;

    // class point cloud writer
    template <
        geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT,
        class vtkGridWriterT>
    requires(
        utilities::same_dim_c<cloudT, coordSystemT>
        && utilities::same_dim_c<cloudT, vtkGridWriterT>)
    class PointCloudVTKWriter;

    // class node writer
    template <
        discretization::function_space_c functionSpaceT, geometry::coordinate_system_c coordSystemT,
        class vtkGridWriterT>
    requires(
        utilities::same_dim_c<functionSpaceT, coordSystemT>
        && utilities::same_dim_c<functionSpaceT, vtkGridWriterT>)
    class NodeVTKWriter;

    // class field writer
    template <class gridWriterT, geometry::coordinate_system_c coordSystemT>
    class FieldVTKWriter;

#ifdef WITH_PARALLEL_VTK
    // class parallel grid writer
    template <int D>
    class ParallelGridVTKWriter;
#endif    // WITH_PARALLEL_VTK
}


// end of file
