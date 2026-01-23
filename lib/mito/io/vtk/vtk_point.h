// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <geometry::coordinates_c coordT>
    auto insert_vtk_point(const coordT &, vtkSmartPointer<vtkPoints> &) -> void;

    template <>
    auto insert_vtk_point(
        const geometry::coordinates_t<3, geometry::CARTESIAN> & coord,
        vtkSmartPointer<vtkPoints> & pointsVtk) -> void
    {
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coord[0], coord[1], coord[2]);
    }

    template <>
    auto insert_vtk_point(
        const geometry::coordinates_t<2, geometry::CARTESIAN> & coord,
        vtkSmartPointer<vtkPoints> & pointsVtk) -> void
    {
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coord[0], coord[1], 0.);
    }

    template <>
    auto insert_vtk_point(
        const geometry::coordinates_t<1, geometry::CARTESIAN> & coord,
        vtkSmartPointer<vtkPoints> & pointsVtk) -> void
    {
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coord[0], 0., 0.);
    }

    template <>
    auto insert_vtk_point(
        const geometry::coordinates_t<3, geometry::SPHERICAL> & coord,
        vtkSmartPointer<vtkPoints> & pointsVtk) -> void
    {
        // cartesian coordinates in 2D
        using cartesian_coord_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;

        // transform {coord} into cartesian coordinates
        auto cartesian_coord = transform_coordinates<cartesian_coord_t>(coord);

        // add the point as new vtk point
        pointsVtk->InsertNextPoint(cartesian_coord[0], cartesian_coord[1], cartesian_coord[2]);
    }

    template <>
    auto insert_vtk_point(
        const geometry::coordinates_t<2, geometry::POLAR> & coord,
        vtkSmartPointer<vtkPoints> & pointsVtk) -> void
    {
        // cartesian coordinates in 2D
        using cartesian_coord_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

        // transform {coord} into cartesian coordinates
        auto cartesian_coord = transform_coordinates<cartesian_coord_t>(coord);

        // add the point as new vtk point
        pointsVtk->InsertNextPoint(cartesian_coord[0], cartesian_coord[1], 0.);
    }

}    // namespace mito::io::vtk


// end of file
