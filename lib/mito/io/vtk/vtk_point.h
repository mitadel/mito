// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
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

}    // namespace mito::io::vtk


// end of file
