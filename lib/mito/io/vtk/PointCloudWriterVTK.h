// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <geometry::point_cloud_c cloudT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<cloudT, coordSystemT>)
    class PointCloudWriterVTK : public GridWriterVTK<cloudT::dim> {

      public:
        // the grid type
        using grid_type = cloudT;

      private:
        // the coordinate system type
        using coord_system_type = coordSystemT;
        // the dimension of the physical space
        static constexpr int D = cloudT::dim;
        // the type of point
        using point_type = typename coord_system_type::point_type;
        // the type of a collection of points
        using points_type = std::unordered_set<point_type, utilities::hash_function<point_type>>;

      private:
        auto _create_vtk_grid(const grid_type & cloud, const coord_system_type & coordinate_system)
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
            this->_grid->SetPoints(pointsVtk);

            // all done
            return;
        }

      public:
        PointCloudWriterVTK(
            std::string filename, const grid_type & cloud, const coord_system_type & coord_system) :
            GridWriterVTK<D>(filename)
        {
            _create_vtk_grid(cloud, coord_system);
        }

      private:
        // a collection of points in the mesh
        points_type _points;
    };

}    // namespace mito::io::vtk


// end of file
