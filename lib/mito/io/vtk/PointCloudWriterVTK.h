// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(cloudT::dim == coordSystemT::coordinates_type::dim)
    class PointCloudWriterVTK : public GridWriterVTK<cloudT::dim> {

      private:
        // the point cloud type
        using cloud_type = cloudT;
        // the coordinate system type
        using coord_system_type = coordSystemT;
        // the dimension of the physical space
        static constexpr int D = cloudT::dim;

      private:
        auto _create_vtk_grid(const cloud_type & cloud, const coord_system_type & coordinate_system)
        {
            // vtk points and cells
            auto pointsVtk = vtkSmartPointer<vtkPoints>::New();

            // get point cloud compositions
            const auto & points = cloud.points();

            // insert the new vtk point
            for (const auto & point : points) {
                _points.insert(point);
                insert_vtk_point(coordinate_system.coordinates(point), pointsVtk);
            }

            // set the grid points
            _grid->SetPoints(pointsVtk);

            // all done
            return;
        }

      public:
        PointCloudWriterVTK(
            std::string filename, const cloud_type & cloud,
            const coord_system_type & coord_system) :
            GridWriterVTK<D>(filename)
        {
            _create_vtk_grid(cloud, coord_system);
        }
    };

}    // namespace mito::io::vtk


// end of file
