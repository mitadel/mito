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
        const geometry::cartesian<3>::coordinates_t & coord, vtkSmartPointer<vtkPoints> & pointsVtk)
        -> void
    {
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coord[0], coord[1], coord[2]);
    }

    template <>
    auto insert_vtk_point(
        const geometry::cartesian<2>::coordinates_t & coord, vtkSmartPointer<vtkPoints> & pointsVtk)
        -> void
    {
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coord[0], coord[1], 0.);
    }

    template <>
    auto insert_vtk_point(
        const geometry::cartesian<1>::coordinates_t & coord, vtkSmartPointer<vtkPoints> & pointsVtk)
        -> void
    {
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coord[0], 0., 0.);
    }

    template <>
    auto insert_vtk_point(
        const geometry::spherical::coordinates_t & coord, vtkSmartPointer<vtkPoints> & pointsVtk)
        -> void
    {
        // cartesian coordinates in 3D
        using cartesian_coord_t = mito::geometry::cartesian<3>::coordinates_t;

        // transform {coord} into cartesian coordinates
        auto cartesian_coord = transform_coordinates<cartesian_coord_t>(coord);

        // add the point as new vtk point
        pointsVtk->InsertNextPoint(cartesian_coord[0], cartesian_coord[1], cartesian_coord[2]);
    }

    template <>
    auto insert_vtk_point(
        const geometry::polar::coordinates_t & coord, vtkSmartPointer<vtkPoints> & pointsVtk)
        -> void
    {
        // cartesian coordinates in 2D
        using cartesian_coord_t = mito::geometry::cartesian<2>::coordinates_t;

        // transform {coord} into cartesian coordinates
        auto cartesian_coord = transform_coordinates<cartesian_coord_t>(coord);

        // add the point as new vtk point
        pointsVtk->InsertNextPoint(cartesian_coord[0], cartesian_coord[1], 0.);
    }

}    // namespace mito::io::vtk


// end of file
